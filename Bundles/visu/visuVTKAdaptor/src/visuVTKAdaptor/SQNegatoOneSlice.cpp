/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "visuVTKAdaptor/SQNegatoOneSlice.hpp"

#include "visuVTKAdaptor/SQImage.hpp"
#include "visuVTKAdaptor/SQImageSlice.hpp"

#include <fwCom/Slot.hpp>
#include <fwCom/Slot.hxx>
#include <fwCom/Slots.hpp>
#include <fwCom/Slots.hxx>

#include <fwData/Color.hpp>
#include <fwData/Image.hpp>
#include <fwData/String.hpp>
#include <fwData/TransferFunction.hpp>

#include <fwDataTools/fieldHelper/Image.hpp>
#include <fwDataTools/fieldHelper/MedicalImageHelpers.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/op/Add.hpp>
#include <fwServices/QtObjectHolder.hpp>
#include <fwServices/QtQmlRegistar.hxx>

#include <fwVtkIO/vtk.hpp>

#include <vtkImageBlend.h>
#include <vtkImageCheckerboard.h>
#include <vtkImageData.h>
#include <vtkImageMapToColors.h>
#include <vtkProperty.h>

namespace visuVTKAdaptor
{

static ::fwServices::QtQmlRegistar<SQNegatoOneSlice>    registar("::visuVTKAdaptor::SNegatoOneSlice");

static const ::fwCom::Slots::SlotKeyType s_UPDATE_SLICE_TYPE_SLOT = "updateSliceType";
static const ::fwCom::Slots::SlotKeyType s_UPDATE_IMAGE_SLOT      = "updateImage";

static const ::fwServices::IService::KeyType s_IMAGE_INOUT = "image";
static const ::fwServices::IService::KeyType s_TF_INOUT    = "tf";

//------------------------------------------------------------------------------

SQNegatoOneSlice::SQNegatoOneSlice() noexcept :
    m_manageImageSource(false),
    m_imageSource(nullptr),
    m_allowAlphaInTF(false),
    m_interpolation(true),
    m_actorOpacity(1.0)
{
    newSlot(s_UPDATE_SLICE_TYPE_SLOT, &SQNegatoOneSlice::updateSliceType, this);
    newSlot(s_UPDATE_IMAGE_SLOT, &SQNegatoOneSlice::updateImage, this);
}

//------------------------------------------------------------------------------

SQNegatoOneSlice::~SQNegatoOneSlice() noexcept
{
    this->unregisterServices();
    this->cleanImageSource();
    std::cout << "Deleted OneSlice" << std::endl;
}

//------------------------------------------------------------------------------

vtkObject* SQNegatoOneSlice::getImageSource()
{
    if ( !m_imageSource )
    {
        OSLM_TRACE(this->getID() << ": Create ImageSource");
        if (!m_imageSourceId.empty())
        {
            m_imageSource = this->getVtkObject(m_imageSourceId);
        }
        else
        {
            m_imageSource       = vtkImageMapToColors::New();
            m_manageImageSource = true;
            std::cout << "Image source = " << m_imageSource << std::endl;
        }
    }

    return m_imageSource;
}

//------------------------------------------------------------------------------

void SQNegatoOneSlice::cleanImageSource()
{
    std::cout << m_manageImageSource << " " << m_imageSource << std::endl;
    if (m_manageImageSource && m_imageSource)
    {
        m_imageSource->Delete();
        m_imageSource = nullptr;
    }
}

//------------------------------------------------------------------------------

std::shared_ptr<::fwRenderVTK::IQAdaptor> SQNegatoOneSlice::getImageSliceAdaptor()
{
    if (m_imageSliceAdaptor.expired())
    {
        OSLM_TRACE(this->getID() << ": Create SImageSlice Adaptor Service");
        ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);
        SLM_ASSERT("Missing image", image);

        // create the srv configuration for objects auto-connection
        auto imgSliceAdaptor = this->registerService< ::visuVTKAdaptor::SQImageSlice>("::visuVTKAdaptor::SImageSlice");
        imgSliceAdaptor->setProperty(s_IMAGE_INOUT, QVariant::fromValue(new ::fwServices::QtObjectHolder(image)));

        imgSliceAdaptor->setRenderService(this->getRenderService());
        imgSliceAdaptor->setRendererId( this->getRendererId() );
        imgSliceAdaptor->setPickerId( this->getPickerId() );
        imgSliceAdaptor->setTransformId( this->getTransformId() );

        imgSliceAdaptor->setVtkImageSource(this->getImageSource());
        imgSliceAdaptor->setInterpolation(m_interpolation);
        imgSliceAdaptor->setActorOpacity(m_actorOpacity);
        imgSliceAdaptor->setOrientation(m_orientation);

        m_imageSliceAdaptor = imgSliceAdaptor;
    }

    return m_imageSliceAdaptor.lock();
}

//------------------------------------------------------------------------------

std::shared_ptr<::fwRenderVTK::IQAdaptor> SQNegatoOneSlice::getImageAdaptor()
{
    if (m_imageAdaptor.expired())
    {
        OSLM_TRACE(this->getID() << ": Create Image Adaptor Service");
        ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);
        SLM_ASSERT("Missing image", image);

        // create the srv configuration for objects auto-connection
        auto imgAdaptor = this->registerService< ::visuVTKAdaptor::SQImage>("::visuVTKAdaptor::SImage");
        imgAdaptor->setProperty(s_IMAGE_INOUT, QVariant::fromValue(new ::fwServices::QtObjectHolder(image)));

        imgAdaptor->setRenderService(this->getRenderService());
        imgAdaptor->setRendererId( this->getRendererId() );
        imgAdaptor->setPickerId( this->getPickerId() );
        imgAdaptor->setTransformId( this->getTransformId() );

        imgAdaptor->setVtkImageRegister(this->getImageSource());

        ::fwData::TransferFunction::sptr tf = this->getInOut< ::fwData::TransferFunction >(s_TF_INOUT);
        if (tf)
        {
            imgAdaptor->setProperty(s_TF_INOUT, QVariant::fromValue(new ::fwServices::QtObjectHolder(tf)));
        }

        imgAdaptor->setImageOpacity(1.);
        imgAdaptor->setAllowAlphaInTF(m_allowAlphaInTF);

        m_imageAdaptor = imgAdaptor;
    }

    return m_imageAdaptor.lock();
}

//------------------------------------------------------------------------------

void SQNegatoOneSlice::starting()
{
    this->initialize();
    if (nullptr == vtkImageBlend::SafeDownCast(this->getImageSource())
        && nullptr == vtkImageCheckerboard::SafeDownCast(this->getImageSource()))
    {
        this->getImageAdaptor()->start();
    }
    this->getImageSliceAdaptor()->start();
}

//------------------------------------------------------------------------------

void SQNegatoOneSlice::stopping()
{
    this->unregisterServices();
    this->cleanImageSource();
}

//------------------------------------------------------------------------------

void SQNegatoOneSlice::updating()
{
    if (nullptr == vtkImageBlend::SafeDownCast(this->getImageSource())
        && nullptr == vtkImageCheckerboard::SafeDownCast(this->getImageSource()))
    {
        this->getImageAdaptor()->update();
    }
    this->getImageSliceAdaptor()->update();
}

//------------------------------------------------------------------------------

void SQNegatoOneSlice::updateSliceType(int from, int to)
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
        auto imgSliceAdaptor = std::dynamic_pointer_cast<::visuVTKAdaptor::SQImageSlice>(srv);

        if (imgSliceAdaptor)
        {
            imgSliceAdaptor->updateSliceType(from, to);
        }
    }
}

//-----------------------------------------------------------------------------

void SQNegatoOneSlice::updateImage()
{
    this->stopping();
    this->starting();
    this->updating();
}

//------------------------------------------------------------------------------

void SQNegatoOneSlice::configuring()
{
/*    this->configureParams();

    const ConfigType config = this->getConfigTree().get_child("config.<xmlattr>");

    const std::string orientation = config.get<std::string>("sliceIndex", "axial");
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

    const std::string tfalpha = config.get<std::string>("tfalpha", "no");
    SLM_ASSERT("'tfalpha' value must be 'yes' or 'no', actual: " + tfalpha, tfalpha == "yes" || tfalpha == "no");
    this->setAllowAlphaInTF(tfalpha == "yes");

    const std::string interpolation = config.get<std::string>("interpolation", "off");
    SLM_ASSERT("'interpolation' value must be 'on' or 'off', actual: " + interpolation,
               interpolation == "on" || interpolation == "off");
    this->setInterpolation(interpolation == "yes");

    this->setVtkImageSourceId( config.get<std::string>("vtkimagesource", ""));

    m_actorOpacity = config.get<double>("actorOpacity", 1.);*/
}

void SQNegatoOneSlice::updateSliceIndex(int axial, int frontal, int sagittal)
{
    for (auto& srv : this->getRegisteredServices())
    {
        auto imgSliceAdaptor = std::dynamic_pointer_cast<::visuVTKAdaptor::SQImageSlice>(srv);

        if (imgSliceAdaptor)
        {
            imgSliceAdaptor->updateSliceIndex(axial, frontal, sagittal);
        }
    }
}


//------------------------------------------------------------------------------

} //namespace visuVTKAdaptor
