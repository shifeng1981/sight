/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "visuVTKAdaptor/SQTexture.hpp"

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
#include <fwServices/QtQmlRegistar.hxx>
#include <fwServices/QtQmlType.hxx>

#include <vtkRenderWindowInteractor.h>
#include <vtkTexture.h>

namespace visuVTKAdaptor
{

static const ::fwServices::QtQmlRegistar<SQTexture> registar("::visuVTKAdaptor::STexture");
static const ::fwServices::QtQmlType<SQTexture> registarType("com.fw4spl.vtk.adaptors", 1, 0, "STexture");


const ::fwCom::Slots::SlotKeyType SQTexture::s_APPLY_TEXTURE_SLOT = "applyTexture";

static const ::fwServices::IService::KeyType s_TEXTURE_INOUT = "texture";

//------------------------------------------------------------------------------

SQTexture::SQTexture() noexcept :
    m_filtering("linear"),
    m_wrapping("repeat"),
    m_lighting(true)
{
}

//------------------------------------------------------------------------------

SQTexture::~SQTexture() noexcept
{
}

//------------------------------------------------------------------------------

void SQTexture::configuring()
{
    this->configureParams();

    m_filtering = get<QString>(m_configMap, "filtering", "linear").toStdString();

    m_wrapping = get<QString>(m_configMap, "wrapping", "repeat").toStdString();

    m_lighting = (get<QString>(m_configMap, "lighting", "yes").toStdString() == "yes");
}

//------------------------------------------------------------------------------

void SQTexture::starting()
{
    this->initialize();
}

//------------------------------------------------------------------------------

void SQTexture::updating()
{
    for(::fwData::Material::sptr material :  m_materialSet)
    {
        applyTexture(material);
    }
}

//------------------------------------------------------------------------------

void SQTexture::stopping()
{
}

//------------------------------------------------------------------------------

void    SQTexture::applyTexture(QObject *material)
{
    std::cout << "Applying texture : " << material << std::endl;
    auto objectHolder = qobject_cast<::fwServices::QtObjectHolder *>(material);

    SLM_ASSERT("Unknown type passed as signal parameter", objectHolder);
    this->applyTexture(std::dynamic_pointer_cast<::fwData::Material>(objectHolder->getObject()));
}

//------------------------------------------------------------------------------

void SQTexture::applyTexture( SPTR(::fwData::Material)_material )
{
    if(m_materialSet.count(_material) == 0)
    {
        m_materialSet.insert(_material);
    }
    ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_TEXTURE_INOUT);

    if (!image)
    {
        return;
    }

    SLM_ASSERT("Texture image is missing.", image);

    ::fwData::mt::ObjectWriteLock matLock(_material);

    {
        ::fwData::mt::ObjectReadLock imLock(image);

        if(!::fwDataTools::fieldHelper::MedicalImageHelpers::checkImageValidity(image))
        {
            return;
        }

        _material->setDiffuseTexture(image);
        if(m_lighting == false)
        {
            _material->setShadingMode(::fwData::Material::AMBIENT);
        }
    }

    ::fwData::Material::FilteringType filtering = ::fwData::Material::LINEAR;
    ::fwData::Material::WrappingType wrapping   = ::fwData::Material::REPEAT;

    if(m_filtering == "nearest")
    {
        filtering = ::fwData::Material::NEAREST;
    }
    else if(m_filtering == "linear")
    {
        filtering = ::fwData::Material::LINEAR;
    }
    else
    {
        OSLM_WARN("SQTexture filtering type unknown or not supported : " << m_filtering);
    }
    _material->setDiffuseTextureFiltering(filtering);

    if(m_wrapping == "repeat")
    {
        wrapping = ::fwData::Material::REPEAT;
    }
    else if(m_wrapping == "clamp")
    {
        wrapping = ::fwData::Material::CLAMP;
    }
    else
    {
        OSLM_WARN("SQTexture wrapping type unknown or not supported : " << m_wrapping);
    }
    _material->setDiffuseTextureWrapping(wrapping);

 /*   ::fwData::Object::ModifiedSignalType::sptr sig;
    sig = _material->signal< ::fwData::Object::ModifiedSignalType >(::fwData::Object::s_MODIFIED_SIG);
    {
        ::fwCom::Connection::Blocker block(sig->getConnection(m_slotUpdate));
        sig->asyncEmit();
    }*/
}

//------------------------------------------------------------------------------

} //namespace visuVTKAdaptor
