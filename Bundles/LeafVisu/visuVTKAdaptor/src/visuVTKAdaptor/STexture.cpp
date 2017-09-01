/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "visuVTKAdaptor/STexture.hpp"

#include <fwCom/Signal.hxx>
#include <fwCom/Slot.hxx>
#include <fwCom/Slots.hxx>

#include <fwData/Image.hpp>
#include <fwData/Material.hpp>
//#include <fwData/Mesh.hpp>
#include <fwData/mt/ObjectReadLock.hpp>
#include <fwData/mt/ObjectWriteLock.hpp>
#include <fwData/Reconstruction.hpp>

#include <fwDataTools/fieldHelper/MedicalImageHelpers.hpp>

#include <fwMedData/ModelSeries.hpp>

#include <fwServices/macros.hpp>

#include <vtkRenderWindowInteractor.h>
#include <vtkTexture.h>

fwServicesRegisterMacro( ::fwRenderVTK::IAdaptor, ::visuVTKAdaptor::STexture);

namespace visuVTKAdaptor
{

const ::fwCom::Slots::SlotKeyType STexture::s_APPLY_TEXTURE_SLOT = "applyTexture";

static const ::fwServices::IService::KeyType s_TEXTURE_INOUT = "texture";

//------------------------------------------------------------------------------

STexture::STexture() noexcept :
    m_filtering("linear"),
    m_wrapping("repeat"),
    m_blending("none"),
    m_lighting(true),
    m_name("diffuse")
{
    newSlot(s_APPLY_TEXTURE_SLOT, &STexture::applyTexture, this );
}

//------------------------------------------------------------------------------

STexture::~STexture() noexcept
{
}

//------------------------------------------------------------------------------

void STexture::configuring()
{
    this->configureParams();

    /* Get texture name */
    m_name = this->getObject()->getID();

    const ConfigType config = this->getConfigTree().get_child("service.config.<xmlattr>");

    m_filtering = config.get<std::string>("filtering", "linear");

    m_wrapping = config.get<std::string>("wrapping", "repeat");

    m_blending = config.get<std::string>("blending", "none");

    m_lighting = (config.get<std::string>("lighting", "yes") == "yes");
}

//------------------------------------------------------------------------------

void STexture::starting()
{
    this->initialize();
}

//------------------------------------------------------------------------------

void STexture::updating()
{
    for(::fwData::Material::sptr material :  m_materialSet)
    {
        applyTexture(material);
    }
}

//------------------------------------------------------------------------------

void STexture::stopping()
{
}

//------------------------------------------------------------------------------

void STexture::applyTexture( SPTR(::fwData::Material)_material )
{
    if(m_materialSet.count(_material) == 0)
    {
        m_materialSet.insert(_material);
    }

    ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_TEXTURE_INOUT);
    SLM_ASSERT("Texture image is missing.", image);

    ::fwData::mt::ObjectWriteLock matLock(_material);

    {
        ::fwData::mt::ObjectReadLock imLock(image);

        if(!::fwDataTools::fieldHelper::MedicalImageHelpers::checkImageValidity(image))
        {
            return;
        }

        _material->setDiffuseTexture(image, m_name);
        if(m_lighting == false)
        {
            _material->setShadingMode(::fwData::Material::AMBIENT);
        }
    }

    ::fwData::DiffuseTexture::FilteringType filtering = ::fwData::DiffuseTexture::LINEAR;
    ::fwData::DiffuseTexture::WrappingType wrapping   = ::fwData::DiffuseTexture::REPEAT;
    ::fwData::DiffuseTexture::BlendingType blending   = ::fwData::DiffuseTexture::NONE;

    if(m_filtering == "nearest")
    {
        filtering = ::fwData::DiffuseTexture::NEAREST;
    }
    else if(m_filtering == "linear")
    {
        filtering = ::fwData::DiffuseTexture::LINEAR;
    }
    else
    {
        OSLM_WARN("STexture filtering type unknown or not supported : " << m_filtering);
    }
    _material->setDiffuseTextureFiltering(filtering, m_name);

    if(m_wrapping == "repeat")
    {
        wrapping = ::fwData::DiffuseTexture::REPEAT;
    }
    else if(m_wrapping == "clamp")
    {
        wrapping = ::fwData::DiffuseTexture::CLAMP;
    }
    else
    {
        OSLM_WARN("STexture wrapping type unknown or not supported : " << m_wrapping);
    }
    _material->setDiffuseTextureWrapping(wrapping, m_name);

    if(m_blending == "none")
    {
        blending = ::fwData::DiffuseTexture::NONE;
    }
    else if(m_blending == "replace")
    {
        blending = ::fwData::DiffuseTexture::REPLACE;
    }
    else if(m_blending == "modulate")
    {
        blending = ::fwData::DiffuseTexture::MODULATE;
    }
    else if(m_blending == "add")
    {
        blending = ::fwData::DiffuseTexture::ADD;
    }
    else if(m_blending == "add_signed")
    {
        blending = ::fwData::DiffuseTexture::ADD_SIGNED;
    }
    else if(m_blending == "interpolate")
    {
        blending = ::fwData::DiffuseTexture::INTERPOLATE;
    }
    else if(m_blending == "subtract")
    {
        blending = ::fwData::DiffuseTexture::SUBTRACT;
    }
    else
    {
        OSLM_WARN("Texture blending mode unknown or not supported : " << m_wrapping);
    }
    _material->setDiffuseTextureBlending(blending, m_name);

    ::fwData::Object::ModifiedSignalType::sptr sig;
    sig = _material->signal< ::fwData::Object::ModifiedSignalType >(::fwData::Object::s_MODIFIED_SIG);
    {
        ::fwCom::Connection::Blocker block(sig->getConnection(m_slotUpdate));
        sig->asyncEmit();
    }
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
