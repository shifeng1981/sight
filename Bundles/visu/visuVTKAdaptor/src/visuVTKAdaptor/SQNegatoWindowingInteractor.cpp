/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "visuVTKAdaptor/SQNegatoWindowingInteractor.hpp"

#include <fwCom/Signal.hpp>
#include <fwCom/Signal.hxx>
#include <fwCom/Signals.hpp>

#include <fwData/Image.hpp>
#include <fwData/Integer.hpp>
#include <fwData/TransferFunction.hpp>

#include <fwDataTools/fieldHelper/Image.hpp>
#include <fwDataTools/fieldHelper/MedicalImageHelpers.hpp>

#include <fwRenderVTK/IAdaptor.hpp>
#include <fwRenderVTK/vtk/fwVtkCellPicker.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/QtQmlRegistar.hxx>

#include <vtkCommand.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderWindowInteractor.h>

#define START_WINDOWING_EVENT vtkCommand::RightButtonPressEvent
#define STOP_WINDOWING_EVENT  vtkCommand::RightButtonReleaseEvent

namespace visuVTKAdaptor
{

static ::fwServices::QtQmlRegistar<SQNegatoWindowingInteractor>    registar(
    "::visuVTKAdaptor::SNegatoWindowingInteractor");

static const ::fwServices::IService::KeyType s_IMAGE_INOUT = "image";
static const ::fwServices::IService::KeyType s_TF_INOUT    = "tf";

//------------------------------------------------------------------------------

class NegatoQWindowingCallback : public vtkCommand
{
public:
    //------------------------------------------------------------------------------

    static NegatoQWindowingCallback* New()
    {
        return new NegatoQWindowingCallback();
    }

    NegatoQWindowingCallback() :
        m_picker(nullptr),
        m_x(0),
        m_y(0),
        m_windowStep(1.),
        m_levelStep(1.),
        m_mouseMoveObserved(false)
    {
        this->PassiveObserverOff();
    }

    ~NegatoQWindowingCallback()
    {

    }

    //------------------------------------------------------------------------------

    virtual void Execute( vtkObject* caller, unsigned long eventId, void*)
    {
        if ( m_mouseMoveObserved || !m_adaptor->getInteractor()->GetShiftKey() )
        {
            if ( eventId == START_WINDOWING_EVENT)
            {
                SLM_ASSERT("m_adaptor not instanced", m_adaptor);
                SLM_ASSERT("m_picker not instanced", m_picker);

                double display[3];

                m_adaptor->getInteractor()->GetEventPosition(m_x, m_y);
                display[0] = m_x;
                display[1] = m_y;
                display[2] = 0;

                if ( m_picker->Pick( display, m_adaptor->getRenderer() ) )
                {
                    assert(!m_mouseMoveObserved);
                    m_adaptor->startWindowing();
                    m_adaptor->getInteractor()->AddObserver(vtkCommand::MouseMoveEvent, this, 1.);
                    m_mouseMoveObserved = true;
                    SetAbortFlag(1);
                    m_adaptor->update();
                }

            }

            else if ( eventId == STOP_WINDOWING_EVENT)
            {
                SLM_ASSERT("m_adaptor not instanced", m_adaptor);
                SLM_ASSERT("m_picker not instanced", m_picker);

                if(m_mouseMoveObserved)
                {
                    m_adaptor->getInteractor()->RemoveObservers(vtkCommand::MouseMoveEvent, this);
                    m_mouseMoveObserved = false;
                    m_adaptor->stopWindowing();
                    m_adaptor->update();
                }
            }
            else if (eventId == vtkCommand::MouseMoveEvent)
            {
                SLM_ASSERT("m_mouseMoveObserved not instanced", m_mouseMoveObserved);
                int x, y;
                m_adaptor->getInteractor()->GetEventPosition(x, y);

                double dx = m_windowStep * ( x - m_x );
                double dy = m_levelStep  * ( m_y - y );

                m_adaptor->updateWindowing(dx, dy);
                m_adaptor->update();
            }
        }
        else if (m_adaptor->getInteractor()->GetShiftKey())
        {
            vtkRenderWindowInteractor* rwi = vtkRenderWindowInteractor::SafeDownCast(caller);
            char* keySym                   = rwi->GetKeySym();
            if(keySym != nullptr)
            {
                if (std::string(keySym) == "R")
                {
                    m_adaptor->resetWindowing();
                }
            }
        }
    }

    //------------------------------------------------------------------------------

    void setAdaptor(SQNegatoWindowingInteractor* adaptor)
    {
        m_adaptor = adaptor;
    }

    //------------------------------------------------------------------------------

    void setPicker( vtkAbstractPropPicker* picker)
    {
        m_picker = picker;
    }

protected:
    SQNegatoWindowingInteractor* m_adaptor;
    vtkAbstractPropPicker* m_picker;

    int m_x;
    int m_y;

    double m_windowStep;
    double m_levelStep;

    bool m_mouseMoveObserved;

};

//------------------------------------------------------------------------------

SQNegatoWindowingInteractor::SQNegatoWindowingInteractor() noexcept :
    m_vtkObserver(nullptr),
    m_initialWindow(0.),
    m_initialLevel(0.),
    m_priority(.6f)
{
}

//------------------------------------------------------------------------------

SQNegatoWindowingInteractor::~SQNegatoWindowingInteractor() noexcept
{
}

//------------------------------------------------------------------------------

void SQNegatoWindowingInteractor::configuring()
{
    this->configureParams();
}

//------------------------------------------------------------------------------

void SQNegatoWindowingInteractor::starting()
{
    this->initialize();

    ::fwData::TransferFunction::sptr tf = this->getInOut< ::fwData::TransferFunction >(s_TF_INOUT);
    if (tf)
    {
        this->setTransferFunction(tf);
    }
    else
    {
        ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);
        SLM_ASSERT("Missing image", image);
        this->createTransferFunction(image);
    }

    NegatoQWindowingCallback* observer = NegatoQWindowingCallback::New();
    observer->setAdaptor( this);
    observer->setPicker(this->getPicker());

    m_vtkObserver = observer;

    this->getInteractor()->AddObserver(START_WINDOWING_EVENT, m_vtkObserver, m_priority);
    this->getInteractor()->AddObserver(STOP_WINDOWING_EVENT, m_vtkObserver, m_priority);
    this->getInteractor()->AddObserver(vtkCommand::KeyPressEvent, m_vtkObserver, m_priority);

    this->updating();
}

//------------------------------------------------------------------------------

void SQNegatoWindowingInteractor::updating()
{
    ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);
    SLM_ASSERT("Missing image", image);

    this->updateImageInfos(image);
    this->requestRender();
}

//------------------------------------------------------------------------------

void SQNegatoWindowingInteractor::stopping()
{
    this->getInteractor()->RemoveObservers(START_WINDOWING_EVENT, m_vtkObserver);
    this->getInteractor()->RemoveObservers(STOP_WINDOWING_EVENT, m_vtkObserver);
    this->getInteractor()->RemoveObservers(vtkCommand::KeyPressEvent, m_vtkObserver);
    m_vtkObserver->Delete();
    m_vtkObserver = nullptr;
    this->removeAllPropFromRenderer();
}

//------------------------------------------------------------------------------

void SQNegatoWindowingInteractor::startWindowing( )
{
    ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);
    SLM_ASSERT("Missing image", image);

    this->updating();

    ::fwData::TransferFunction::sptr tf = this->getTransferFunction();

    m_initialLevel  = tf->getLevel();
    m_initialWindow = tf->getWindow();
}

//------------------------------------------------------------------------------

void SQNegatoWindowingInteractor::stopWindowing( )
{
}

//------------------------------------------------------------------------------

void SQNegatoWindowingInteractor::updateWindowing( double dw, double dl )
{
    ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);
    SLM_ASSERT("Missing image", image);
    ::fwData::TransferFunction::sptr tf = this->getTransferFunction();

    double newWindow = m_initialWindow + dw;
    double newLevel  = m_initialLevel - dl;

    tf->setWindow( newWindow );
    tf->setLevel( newLevel );

    auto sig = tf->signal< ::fwData::TransferFunction::WindowingModifiedSignalType >(
        ::fwData::TransferFunction::s_WINDOWING_MODIFIED_SIG);
    {
        ::fwCom::Connection::Blocker block(sig->getConnection(m_slotUpdateTFWindowing));
        sig->asyncEmit( newWindow, newLevel);
    }

    this->setVtkPipelineModified();
}

//------------------------------------------------------------------------------

void SQNegatoWindowingInteractor::resetWindowing()
{
    ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);
    SLM_ASSERT("Missing image", image);
    ::fwData::TransferFunction::sptr tf = this->getTransferFunction();

    double newWindow = image->getWindowWidth();
    double newLevel  = image->getWindowCenter();

    tf->setWindow( newWindow );
    tf->setLevel( newLevel );

    auto sig = tf->signal< ::fwData::TransferFunction::WindowingModifiedSignalType >(
        ::fwData::TransferFunction::s_WINDOWING_MODIFIED_SIG);
    {
        ::fwCom::Connection::Blocker block(sig->getConnection(m_slotUpdateTFWindowing));
        sig->asyncEmit( newWindow, newLevel);
    }

    this->setVtkPipelineModified();
}

//------------------------------------------------------------------------------

} //namespace visuVTKAdaptor
