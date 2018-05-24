/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "visuVTKAdaptor/SQNegatoMPR.hpp"

#include "visuVTKAdaptor/SQNegatoOneSlice.hpp"
#include "visuVTKAdaptor/SQNegatoSlicingInteractor.hpp"
#include "visuVTKAdaptor/SQNegatoWindowingInteractor.hpp"
#include "visuVTKAdaptor/SQSlicesCursor.hpp"
#include "visuVTKAdaptor/SQImageSlice.hpp"
#include "visuVTKAdaptor/SQProbeCursor.hpp"

#include <fwCom/Slot.hpp>
#include <fwCom/Slot.hxx>
#include <fwCom/Slots.hpp>
#include <fwCom/Slots.hxx>

#include <fwVtkIO/vtk.hpp>

#include <fwData/Boolean.hpp>
#include <fwData/Image.hpp>
#include <fwData/Integer.hpp>
#include <fwData/String.hpp>

#include <fwDataTools/fieldHelper/Image.hpp>
#include <fwDataTools/fieldHelper/MedicalImageHelpers.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/op/Add.hpp>
#include <fwServices/registry/Proxy.hpp>
#include <fwServices/registry/ActiveWorkers.hpp>
#include <fwServices/QtQmlType.hxx>

#include <fwTools/fwID.hpp>

#include <vtkProperty.h>
#include <vtkImageData.h>


#include <string>

namespace visuVTKAdaptor
{
static const ::fwServices::QtQmlType<SQNegatoMPR>  registar("com.fw4spl.vtk.adaptors", 1, 0, "SNegatoMPR");

static const ::fwCom::Slots::SlotKeyType s_UPDATE_SLICE_TYPE_SLOT   = "updateSliceType";
static const ::fwCom::Slots::SlotKeyType s_UPDATE_SLICE_MODE_SLOT   = "updateSliceMode";
static const ::fwCom::Slots::SlotKeyType s_SHOW_SLICE_SLOT          = "showSlice";
static const ::fwCom::Slots::SlotKeyType s_SET_CROSS_SCALE_SLOT     = "setCrossScale";
static const ::fwCom::Slots::SlotKeyType S_CHANGE_IMAGE_SOURCE_SLOT = "changeImageSource";

static const std::string s_slicingStartingProxy = "slicingStartingProxy";
static const std::string s_slicingStoppingProxy = "slicingStoppingProxy";

const ::fwServices::IService::KeyType SQNegatoMPR::s_IMAGE_INOUT = "image";
const ::fwServices::IService::KeyType SQNegatoMPR::s_TF_INOUT    = "tf";

//------------------------------------------------------------------------------

SQNegatoMPR::SQNegatoMPR() noexcept :
    m_allowAlphaInTF(false),
    m_interpolation(false),
    m_actorOpacity(1.0),
    m_3dModeEnabled( ::boost::logic::indeterminate ),
    m_sliceMode(THREE_SLICES),
    m_backupedSliceMode(THREE_SLICES),
    m_image(nullptr)
{

    ::fwCom::HasSlots::m_slots.setWorker( m_associatedWorker );
}

//------------------------------------------------------------------------------

SQNegatoMPR::~SQNegatoMPR() noexcept
{

}

//------------------------------------------------------------------------------

void SQNegatoMPR::starting()
{
    std::cout << "Starting" << std::endl;
    this->initialize();
    this->updating();
}

//------------------------------------------------------------------------------

void SQNegatoMPR::stopping()
{
    SLM_ASSERT("Missing image", m_image || m_image->getObject());

    //disconnect proxy
    ::fwServices::registry::Proxy::sptr proxy = ::fwServices::registry::Proxy::getDefault();

    for (auto srv : this->getRegisteredServices())
    {
        std::shared_ptr<SQNegatoSlicingInteractor> negatoSlicingInteractor = std::dynamic_pointer_cast<SQNegatoSlicingInteractor>(srv);
        std::shared_ptr<SQSlicesCursor> sliceCursor                        = std::dynamic_pointer_cast<SQSlicesCursor>(srv);
        if (negatoSlicingInteractor)
        {
            proxy->disconnect(m_slicingStartingProxy, negatoSlicingInteractor->signal(
                                  SQNegatoSlicingInteractor::s_SLICING_STARTED_SIG));
            proxy->disconnect(m_slicingStoppingProxy, negatoSlicingInteractor->signal(
                                  SQNegatoSlicingInteractor::s_SLICING_STOPPED_SIG));
        }

        if (sliceCursor)
        {
            proxy->disconnect(m_slicingStartingProxy, sliceCursor->slot(
                                  SQSlicesCursor::s_SHOW_FULL_CROSS_SLOT));

            proxy->disconnect(m_slicingStoppingProxy, sliceCursor->slot(
                                  SQSlicesCursor::s_SHOW_NORMAL_CROSS_SLOT));
        }
    }

    this->unregisterServices();
    this->requestRender();
}

//------------------------------------------------------------------------------

void SQNegatoMPR::updating()
{
    if (!m_image || !m_image->getObject())
    {
        return ;
    }
    this->stopping();

    SLM_ASSERT("Missing image", m_image || m_image->getObject());
    auto image = std::dynamic_pointer_cast<::fwData::Image>(m_image->getObject());
    bool imageIsValid = ::fwDataTools::fieldHelper::MedicalImageHelpers::checkImageValidity( image );

    if ( imageIsValid)
    {
        if(this->getSliceMode() != NO_SLICE)
        {
            if(this->getSliceMode() == ONE_SLICE)
            {
                this->addAdaptor("::visuVTKAdaptor::SNegatoOneSlice", m_orientation);
            }
            else if(this->getSliceMode() == THREE_SLICES)
            {
                this->addAdaptor("::visuVTKAdaptor::SNegatoOneSlice", X_AXIS);
                this->addAdaptor("::visuVTKAdaptor::SNegatoOneSlice", Y_AXIS);
                this->addAdaptor("::visuVTKAdaptor::SNegatoOneSlice", Z_AXIS);
            }

            /*std::shared_ptr<::fwRenderVTK::IQAdaptor> sliceCursor;
            std::shared_ptr<::fwRenderVTK::IQAdaptor> negatoSlicingInteractor;
            this->addAdaptor("::visuVTKAdaptor::SNegatoWindowingInteractor");
            negatoSlicingInteractor = this->addAdaptor("::visuVTKAdaptor::SNegatoSlicingInteractor", m_orientation);
            sliceCursor             = this->addAdaptor("::visuVTKAdaptor::SSlicesCursor", m_orientation);
            this->addAdaptor("::visuVTKAdaptor::SProbeCursor", m_orientation);

            // Connect slicing signals/slots from NegatoSlicingInteractor to SlicesCursor using the image slicing proxy
            ::fwServices::registry::Proxy::sptr proxy = ::fwServices::registry::Proxy::getDefault();
            m_slicingStartingProxy                    = image->getID() + s_slicingStartingProxy;
            m_slicingStoppingProxy                    = image->getID() + s_slicingStoppingProxy;
            proxy->connect(m_slicingStartingProxy, negatoSlicingInteractor->signal(
                               SQNegatoSlicingInteractor::s_SLICING_STARTED_SIG));
            proxy->connect(m_slicingStartingProxy, sliceCursor->slot(
                               SQSlicesCursor::s_SHOW_FULL_CROSS_SLOT));

            proxy->connect(m_slicingStoppingProxy, negatoSlicingInteractor->signal(
                               SQNegatoSlicingInteractor::s_SLICING_STOPPED_SIG));
            proxy->connect(m_slicingStoppingProxy, sliceCursor->slot(
                               SQSlicesCursor::s_SHOW_NORMAL_CROSS_SLOT));
            m_sliceCursor = sliceCursor;*/
        }
        if(this->is3dModeEnabled())
        {
            this->addAdaptor("::visuVTKAdaptor::SMedical3DCamera", m_orientation);
        }
        else if(!this->is3dModeEnabled())
        {
            this->addAdaptor("::visuVTKAdaptor::SSliceFollowerCamera", m_orientation);
        }
        else
        {
            SLM_TRACE("No 2D/3D mode specified.");
        }
        this->setVtkPipelineModified();
        this->requestRender();
    }
}

//------------------------------------------------------------------------------

void SQNegatoMPR::swapping(const KeyType& key)
{
/*    if (key == s_TF_INOUT)
    {
        ::fwData::TransferFunction::sptr tf = this->getInOut< ::fwData::TransferFunction >(s_TF_INOUT);
        this->setTransferFunction(tf);

        for (const auto& srv: this->getRegisteredServices())
        {
            ::fwServices::IService::sptr service = srv.lock();
            if (tf)
            {
                service->registerInOut(tf, s_TF_INOUT, true, true);
                service->swapKey(s_TF_INOUT, nullptr);
            }
            else if(::fwServices::OSR::isRegistered(s_TF_INOUT, AccessType::INOUT, service))
            {
                ::fwServices::OSR::unregisterService(s_TF_INOUT, AccessType::INOUT, service);
                service->swapKey(s_TF_INOUT, nullptr);
            }
        }
    }*/
}

//-----------------------------------------------------------------------------

void SQNegatoMPR::updateSliceType(int from, int to)
{
    if( to == static_cast<int>(m_orientation) )
    {
        setOrientation( static_cast< Orientation >( from ));
    }
    else if(from == static_cast<int>(m_orientation))
    {
        setOrientation( static_cast< Orientation >( to ));
    }
    for (auto& srv : this->getRegisteredServices())
    {
        auto imageSlice = std::dynamic_pointer_cast<::visuVTKAdaptor::SQNegatoOneSlice>(srv);
        auto negatoSlicingInteractor = std::dynamic_pointer_cast<::visuVTKAdaptor::SQNegatoSlicingInteractor>(srv);
        auto sliceCursor = std::dynamic_pointer_cast<::visuVTKAdaptor::SQSlicesCursor>(srv);

        if (imageSlice)
        {
            imageSlice->updateSliceType(from, to);
        }
        if (negatoSlicingInteractor)
        {
            negatoSlicingInteractor->updateSliceType(from, to);
        }
        if (sliceCursor)
        {
             sliceCursor->updateSliceType(from, to);
        }
    }

}

//-----------------------------------------------------------------------------

void SQNegatoMPR::updateSliceMode(int mode)
{
    switch(mode)
    {
        case 0:
        {
            this->setSliceMode(NO_SLICE);
            break;
        }
        case 1:
        {
            this->setSliceMode(ONE_SLICE);
            break;
        }
        case 3:
        {
            this->setSliceMode(THREE_SLICES);
            break;
        }
        default:
        {
            FW_RAISE("Slice mode " << mode << " is not implemented.");
        }
    }
    m_backupedSliceMode = this->getSliceMode();
    this->updating();
}
//-----------------------------------------------------------------------------

void SQNegatoMPR::showSlice(bool isShown)
{
    if(isShown)
    {
        this->setSliceMode(m_backupedSliceMode);
    }
    else
    {
        m_backupedSliceMode = this->getSliceMode();
        this->setSliceMode(NO_SLICE);
    }
    this->updating();
}

//-----------------------------------------------------------------------------

void SQNegatoMPR::setCrossScale(double scale)
{
    if (m_sliceCursor)
    {
        ::fwCom::SlotBase::sptr slot = m_sliceCursor->slot(s_SET_CROSS_SCALE_SLOT);
        slot->asyncRun(scale);
    }
}

//-----------------------------------------------------------------------------

void SQNegatoMPR::changeImageSource(std::string _value, std::string _key)
{
    if( _key == "ImageSource" )
    {
        // Select the right algorithm
        m_imageSourceId = _value;

        this->updating();
    }
}

//------------------------------------------------------------------------------

void SQNegatoMPR::configuring()
{
    this->configureParams();

    if (m_configMap.count("mode"))
    {
        std::string value = get<QString>(m_configMap, "mode").toStdString();
        std::transform(value.begin(), value.end(), value.begin(), tolower);
        SLM_ASSERT("Bad value '" + value + "' for attribute mode, it should either be '2d' or '3d'.",
                   value == "3d" || value == "2d");
        this->set3dMode(value == "3d");
    }
    if (m_configMap.count("slices"))
    {
        const std::string value = get<QString>(m_configMap, "slices").toStdString();

        if(value == "0")
        {
            this->setSliceMode(NO_SLICE);
        }
        else if(value == "1")
        {
            this->setSliceMode(ONE_SLICE);
        }
        else if(value == "3")
        {
            this->setSliceMode(THREE_SLICES);
        }
        else
        {
            SLM_FATAL("'slice' value must be '0', '1' or '3', actual: " + value);
        }
        m_backupedSliceMode = this->getSliceMode();
    }

    const std::string orientation = get<QString>(m_configMap, "sliceIndex", "axial").toStdString();
    if(orientation == "axial" )
    {
        m_orientation = Z_AXIS;
    }
    else if(orientation == "frontal" )
    {
        m_orientation = Y_AXIS;
    }
    else if(orientation == "sagittal" )
    {
        m_orientation = X_AXIS;
    }

    const std::string tfalpha = get<QString>(m_configMap, "tfalpha", "no").toStdString();
    SLM_ASSERT("'tfalpha' value must be 'yes' or 'no', actual: " + tfalpha, tfalpha == "yes" || tfalpha == "no");
    this->setAllowAlphaInTF(tfalpha == "yes");

    const std::string interpolation = get<QString>(m_configMap, "interpolation", "off").toStdString();
    SLM_ASSERT("'interpolation' value must be 'on' or 'off', actual: " + interpolation,
               interpolation == "on" || interpolation == "off");
    this->setInterpolation(interpolation == "yes");

    this->setVtkImageSourceId(get<QString>(m_configMap, "vtkimagesource", "").toStdString());

    m_actorOpacity = get<double>(m_configMap, "actorOpacity", 1.);
}

//------------------------------------------------------------------------------

void SQNegatoMPR::setSliceMode(SliceMode sliceMode)
{
    if(m_sliceMode != sliceMode)
    {
        m_sliceMode = sliceMode;
    }
}

//------------------------------------------------------------------------------

SQNegatoMPR::SliceMode SQNegatoMPR::getSliceMode() const
{
    return m_sliceMode;
}

//------------------------------------------------------------------------------

::boost::logic::tribool SQNegatoMPR::is3dModeEnabled() const
{
    return m_3dModeEnabled;
}

//------------------------------------------------------------------------------

void SQNegatoMPR::set3dMode( bool enabled )
{
    m_3dModeEnabled = enabled;
}

//------------------------------------------------------------------------------

std::shared_ptr<::fwRenderVTK::IQAdaptor> SQNegatoMPR::addAdaptor(const std::string& adaptorType, int axis)
{
    SLM_ASSERT("Missing image", m_image || m_image->getObject());

    // create the srv configuration for objects auto-connection
    auto service = this->registerService< ::fwRenderVTK::IQAdaptor>(adaptorType);

    SLM_ASSERT("Service `" + adaptorType + "` not found.", service);
    // register image
    service->setProperty(s_IMAGE_INOUT, QVariant::fromValue(new ::fwServices::QtObjectHolder(m_image->getObject())));

    if(axis >= 0)
    {
        auto adaptorSrv = ::fwDataTools::helper::MedicalImageAdaptor::dynamicCast(service);
        SLM_ASSERT("adaptorSrv not instanced", adaptorSrv);
        adaptorSrv->setOrientation((Orientation) axis);
    }

    auto negatoAdaptor = std::dynamic_pointer_cast<::visuVTKAdaptor::SQNegatoOneSlice>(service);

    if (negatoAdaptor)
    {
        negatoAdaptor->setAllowAlphaInTF(m_allowAlphaInTF);
        negatoAdaptor->setInterpolation(m_interpolation);
        if (!m_imageSourceId.empty())
        {
            negatoAdaptor->setVtkImageSourceId(m_imageSourceId);
        }
        negatoAdaptor->setActorOpacity(m_actorOpacity);
    }

    ::fwData::TransferFunction::sptr tf = this->getInOut< ::fwData::TransferFunction >(s_TF_INOUT);
    if (tf)
    {
        // register the TF as optional
        //service->registerInOut(tf, s_TF_INOUT, true, true);
    }

    service->setRenderService(this->getRenderService());
    service->setRendererId( this->getRendererId() );
    service->setPickerId( this->getPickerId() );
    service->setTransformId( this->getTransformId() );

    service->start();
    service->update();

    return service;
}

void    SQNegatoMPR::updateSliceIndex(int axial, int frontal, int sagittal)
{
    for (auto& srv : this->getRegisteredServices())
    {
        auto imageSlice = std::dynamic_pointer_cast<::visuVTKAdaptor::SQNegatoOneSlice>(srv);
        auto negatoSlicingInteractor = std::dynamic_pointer_cast<::visuVTKAdaptor::SQNegatoSlicingInteractor>(srv);
        auto probeCursor = std::dynamic_pointer_cast<::visuVTKAdaptor::SQProbeCursor>(srv);
        auto sliceCursor = std::dynamic_pointer_cast<::visuVTKAdaptor::SQSlicesCursor>(srv);

        if (imageSlice)
        {
            imageSlice->updateSliceIndex(axial, frontal, sagittal);
        }
        if (negatoSlicingInteractor)
        {
            negatoSlicingInteractor->updateSliceIndex(axial, frontal, sagittal);
        }
        if (probeCursor)
        {
            probeCursor->updateSliceIndex(axial, frontal, sagittal);
        }
        if (sliceCursor)
        {
             sliceCursor->updateSliceIndex(axial, frontal, sagittal);
        }
    }
}

//------------------------------------------------------------------------------

} //namespace visuVTKAdaptor
