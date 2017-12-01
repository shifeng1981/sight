/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "visuVTKAdaptor/STexture.hpp"

#include <fwCom/Signal.hxx>
#include <fwCom/Slot.hxx>
#include <fwCom/Slots.hxx>

#include <fwData/Image.hpp>
#include <fwData/Material.hpp>
#include <fwData/mt/ObjectReadLock.hpp>
#include <fwData/mt/ObjectWriteLock.hpp>
#include <fwData/Reconstruction.hpp>

#include <fwDataTools/fieldHelper/MedicalImageHelpers.hpp>

#include <fwMedData/ModelSeries.hpp>

#include <fwServices/macros.hpp>

#include <fwVtkIO/vtk.hpp>

#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include <vtkTexture.h>

fwServicesRegisterMacro( ::fwRenderVTK::IAdaptor, ::visuVTKAdaptor::STexture);

namespace visuVTKAdaptor
{

static const ::fwServices::IService::KeyType s_TEXTURE_INOUT = "texture";

//------------------------------------------------------------------------------

STexture::STexture() noexcept :
    m_filtering("linear"),
    m_wrapping("repeat"),
    m_blending("none"),
    m_lighting(true),
    m_name("")
{
}

//------------------------------------------------------------------------------

STexture::~STexture() noexcept
{
}

//------------------------------------------------------------------------------

void STexture::configuring()
{
    this->configureParams();

    const ConfigType config = this->getConfigTree().get_child("config.<xmlattr>");

    m_name = config.get<std::string>("name", "");
    SLM_ASSERT("The 'name' attribute is missing. It is mandatory to be able to reference the texture.",
               !(m_name.empty()));

    m_filtering = config.get<std::string>("filtering", "linear");
    m_wrapping  = config.get<std::string>("wrapping", "repeat");
    m_blending  = config.get<std::string>("blending", "none");
    m_lighting  = (config.get<std::string>("lighting", "yes") == "yes");
}

//------------------------------------------------------------------------------

void STexture::starting()
{
    this->initialize();

    // Get or create a vtkTexture of the specified name
    vtkTexture* vtkTex = this->getRenderService()->addVtkTexture("Texture_" + m_name);

    vtkSmartPointer< vtkImageData > vtkImage = vtkSmartPointer< vtkImageData >::New();
    vtkImage->SetDimensions(1, 1, 1);
    vtkImage->AllocateScalars(VTK_UNSIGNED_CHAR, 4);

    unsigned char* pixel = static_cast<unsigned char*>(vtkImage->GetScalarPointer(0, 0, 0));
    pixel[0] = 255;
    pixel[1] = 255;
    pixel[2] = 255;
    pixel[3] = 0;

    vtkTex->SetInputData(vtkImage);

    // Set interpolation to nearest by default
    vtkTex->SetInterpolate( 0 );

    // If the user wants a linear interpolation, apply it
    if(m_filtering == "linear")
    {
        vtkTex->SetInterpolate( 1 );
    }

    // Set default values on for wrapping/clamping
    vtkTex->SetRepeat( 0 );
    vtkTex->SetEdgeClamp( 0 );
    if(m_wrapping == "repeat")
    {
        vtkTex->SetRepeat( 1 );
    }
    else if(m_wrapping == "clamp")
    {
        vtkTex->SetEdgeClamp( 1 );
    }

    if(m_blending == "none")
    {
        vtkTex->SetBlendingMode(::vtkTexture::VTK_TEXTURE_BLENDING_MODE_NONE);
    }
    else if(m_blending == "replace")
    {
        vtkTex->SetBlendingMode(::vtkTexture::VTK_TEXTURE_BLENDING_MODE_REPLACE);
    }
    else if(m_blending == "modulate")
    {
        vtkTex->SetBlendingMode(::vtkTexture::VTK_TEXTURE_BLENDING_MODE_MODULATE);
    }
    else if(m_blending == "add")
    {
        vtkTex->SetBlendingMode(::vtkTexture::VTK_TEXTURE_BLENDING_MODE_ADD);
    }
    else if(m_blending == "add_signed")
    {
        vtkTex->SetBlendingMode(::vtkTexture::VTK_TEXTURE_BLENDING_MODE_ADD_SIGNED);
    }
    else if(m_blending == "interpolate")
    {
        vtkTex->SetBlendingMode(::vtkTexture::VTK_TEXTURE_BLENDING_MODE_INTERPOLATE);
    }
    else if(m_blending == "subtract")
    {
        vtkTex->SetBlendingMode(::vtkTexture::VTK_TEXTURE_BLENDING_MODE_SUBTRACT);
    }
}

//------------------------------------------------------------------------------

void STexture::updating()
{
    ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_TEXTURE_INOUT);
    SLM_ASSERT("Texture image is missing.", image);

    if(image)
    {
        ::fwData::mt::ObjectReadLock imLock(image);

        if(!::fwDataTools::fieldHelper::MedicalImageHelpers::checkImageValidity(image))
        {
            return;
        }

        vtkSmartPointer< vtkImageData > vtkImage = vtkSmartPointer< vtkImageData >::New();
        ::fwVtkIO::toVTKImage( image, vtkImage );

        vtkTexture* vtkTex = this->getRenderService()->addVtkTexture("Texture_" + m_name);
        vtkTex->SetInputData(vtkImage);

        this->requestRender();
    }
}

//------------------------------------------------------------------------------

void STexture::stopping()
{
}

//------------------------------------------------------------------------------

::fwServices::IService::KeyConnectionsMap STexture::getAutoConnections() const
{
    KeyConnectionsMap connections;
    connections.push(s_TEXTURE_INOUT, ::fwData::Image::s_MODIFIED_SIG, s_UPDATE_SLOT);
    connections.push(s_TEXTURE_INOUT, ::fwData::Image::s_BUFFER_MODIFIED_SIG, s_UPDATE_SLOT);

    return connections;
}

//------------------------------------------------------------------------------

} //namespace visuVTKAdaptor
