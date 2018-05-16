/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "visuVTKAdaptor/SQTransform.hpp"

#include <fwCom/Signal.hpp>
#include <fwCom/Signal.hxx>

#include <fwData/Boolean.hpp>
#include <fwData/Material.hpp>
#include <fwData/mt/ObjectReadLock.hpp>
#include <fwData/mt/ObjectWriteLock.hpp>
#include <fwData/Reconstruction.hpp>
#include <fwData/TransformationMatrix3D.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/QtQmlRegistar.hxx>

#include <vtkCommand.h>
#include <vtkMatrix4x4.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkTransform.h>

class TransformCallback : public ::vtkCommand
{
public:

    //------------------------------------------------------------------------------

    static TransformCallback* New(::visuVTKAdaptor::SQTransform* adaptor)
    {
        TransformCallback* cb = new TransformCallback;
        cb->m_adaptor = adaptor;
        return cb;
    }

    TransformCallback() :
        m_adaptor(nullptr)
    {
    }
    ~TransformCallback()
    {
    }

    //------------------------------------------------------------------------------

    virtual void Execute( ::vtkObject* pCaller, unsigned long, void* )
    {
        m_adaptor->updateFromVtk();
    }

    ::visuVTKAdaptor::SQTransform* m_adaptor;
};

namespace visuVTKAdaptor
{

static ::fwServices::QtQmlRegistar<SQTransform> registar("::visuVTKAdaptor::STranform");

const ::fwServices::IService::KeyType SQTransform::s_TM3D_INOUT = "tm3d";

//------------------------------------------------------------------------------

SQTransform::SQTransform() noexcept :
    m_transform(nullptr),
    m_transformCommand(TransformCallback::New(this))
{
}

//------------------------------------------------------------------------------

SQTransform::~SQTransform() noexcept
{
    if( m_transformCommand )
    {
        m_transformCommand->Delete();
    }
    m_transformCommand = nullptr;
}

//------------------------------------------------------------------------------

void SQTransform::configuring()
{
    this->configureParams();

    m_parentId = get< QString >(m_configMap, "parent", "").toStdString();
}

//------------------------------------------------------------------------------

void SQTransform::starting()
{
    this->initialize();

    if(!m_transformId.empty())
    {
        m_renderService->getOrAddVtkTransform(m_transformId);
    }
    if(!m_parentId.empty())
    {
        m_parentTransform = m_renderService->getOrAddVtkTransform(m_parentId);
    }

    this->updating();
    this->getTransform()->AddObserver( ::vtkCommand::ModifiedEvent, m_transformCommand );
}

//------------------------------------------------------------------------------

void SQTransform::updateFromVtk()
{
    vtkTransform* vtkTrf = this->getTransform();

    vtkTrf->RemoveObserver( m_transformCommand );
    ::fwData::TransformationMatrix3D::sptr trf = this->getInOut< ::fwData::TransformationMatrix3D >(s_TM3D_INOUT);
    SLM_ASSERT("matrix '" + s_TM3D_INOUT + "' is not defined", trf);

    vtkMatrix4x4* mat = nullptr;

    if(m_parentTransform)
    {
        // Get the matrix before concatenation
        mat = vtkTrf->GetConcatenatedTransform(0)->GetMatrix();
    }
    else
    {
        mat = vtkTrf->GetMatrix();
    }

    {
        ::fwData::mt::ObjectWriteLock lock(trf);
        for(int lt = 0; lt < 4; lt++)
        {
            for(int ct = 0; ct < 4; ct++)
            {
                trf->setCoefficient(static_cast<size_t>(lt), static_cast<size_t>(ct), mat->GetElement(lt, ct));
            }
        }
    }

    vtkTrf->AddObserver( ::vtkCommand::ModifiedEvent, m_transformCommand );
}

//------------------------------------------------------------------------------

void SQTransform::updating()
{
    vtkTransform* vtkTrf = this->getTransform();

    vtkTrf->RemoveObserver( m_transformCommand );
    ::fwData::TransformationMatrix3D::sptr trf = this->getInOut< ::fwData::TransformationMatrix3D >(s_TM3D_INOUT);
    SLM_ASSERT("matrix '" + s_TM3D_INOUT + "' is not defined", trf);

    vtkMatrix4x4* mat = vtkMatrix4x4::New();

    {
        ::fwData::mt::ObjectReadLock lock(trf);
        for(int lt = 0; lt < 4; lt++)
        {
            for(int ct = 0; ct < 4; ct++)
            {
                mat->SetElement(lt, ct, trf->getCoefficient(static_cast<size_t>(lt), static_cast<size_t>(ct)));
            }
        }
    }

    if(m_parentTransform)
    {
        vtkTrf->PostMultiply();
        vtkTrf->SetMatrix( mat );
        vtkTrf->Concatenate( m_parentTransform );
    }
    else
    {
        vtkTrf->SetMatrix(mat);
    }

    vtkTrf->Modified();
    vtkTrf->AddObserver( ::vtkCommand::ModifiedEvent, m_transformCommand );
    mat->Delete();
    this->setVtkPipelineModified();
    this->requestRender();
}

//------------------------------------------------------------------------------

void SQTransform::setTransform(vtkTransform* t)
{
    if ( m_transform != t )
    {
        if (m_transform)
        {
            m_transform->Delete();
        }
        if(t)
        {
            t->Register(nullptr);
        }
    }
    m_transform = t;
}

//------------------------------------------------------------------------------

vtkTransform* SQTransform::getTransform()
{
    vtkTransform* t = m_transform;
    if (t == 0)
    {
        t = this->IQAdaptor::getTransform();
    }
    return t;
}

//------------------------------------------------------------------------------

void SQTransform::swapping()
{
    this->updating();
}

//------------------------------------------------------------------------------

void SQTransform::stopping()
{
    this->getTransform()->RemoveObserver(m_transformCommand);
}

} //namespace visuVTKAdaptor
