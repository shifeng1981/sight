/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwData/Material.hpp"

#include "fwData/Exception.hpp"
#include "fwData/registry/macros.hpp"

#include <fwCom/Signal.hpp>
#include <fwCom/Signal.hxx>

fwDataRegisterMacro( ::fwData::Material );

namespace fwData
{

//------------------------------------------------------------------------------

const ::fwCom::Signals::SignalKeyType Material::s_ADDED_TEXTURE_SIG   = "addedTexture";
const ::fwCom::Signals::SignalKeyType Material::s_REMOVED_TEXTURE_SIG = "removedTexture";

//------------------------------------------------------------------------------

Material::Material(::fwData::Object::Key key) :
    m_shadingMode(PHONG),
    m_representationMode(SURFACE),
    m_optionsMode(STANDARD),
    m_ambient( Color::New(0.05f, 0.05f, 0.05f, 1.f) ),
    m_diffuse( Color::New() )
{
    newSignal< AddedTextureSignalType >(s_ADDED_TEXTURE_SIG);
    newSignal< RemovedTextureSignalType >(s_REMOVED_TEXTURE_SIG);
}

//------------------------------------------------------------------------------

Material::~Material()
{
}

//------------------------------------------------------------------------------

void Material::shallowCopy(const Object::csptr& _source )
{
    Material::csptr other = Material::dynamicConstCast(_source);
    FW_RAISE_EXCEPTION_IF( ::fwData::Exception(
                               "Unable to copy" + (_source ? _source->getClassname() : std::string("<NULL>"))
                               + " to " + this->getClassname()), !bool(other) );
    this->fieldShallowCopy( _source );

    m_ambient        = other->m_ambient;
    m_diffuse        = other->m_diffuse;
    m_diffuseTexture = other->m_diffuseTexture;

    m_shadingMode        = other->m_shadingMode;
    m_representationMode = other->m_representationMode;
    m_optionsMode        = other->m_optionsMode;
}

//------------------------------------------------------------------------------

void Material::cachedDeepCopy(const Object::csptr& _source, DeepCopyCacheType& cache)
{
    Material::csptr other = Material::dynamicConstCast(_source);
    FW_RAISE_EXCEPTION_IF( ::fwData::Exception(
                               "Unable to copy" + (_source ? _source->getClassname() : std::string("<NULL>"))
                               + " to " + this->getClassname()), !bool(other) );
    this->fieldDeepCopy( _source, cache );

    m_ambient        = ::fwData::Object::copy( other->m_ambient, cache );
    m_diffuse        = ::fwData::Object::copy( other->m_diffuse, cache );
    m_diffuseTexture = other->m_diffuseTexture;

    m_shadingMode        = other->m_shadingMode;
    m_representationMode = other->m_representationMode;
    m_optionsMode        = other->m_optionsMode;
}

//------------------------------------------------------------------------------

Color::sptr Material::ambient() const
{
    return m_ambient;
}

//------------------------------------------------------------------------------

Color::sptr Material::diffuse() const
{
    return m_diffuse;
}

//------------------------------------------------------------------------------

Image::sptr Material::getDiffuseTexture(const std::string& name) const
{
    DiffuseTextureMap::const_iterator it = m_diffuseTexture.find(name);
    if(it != m_diffuseTexture.end())
    {
        return it->second->getImage();
    }

    return nullptr;
}

//------------------------------------------------------------------------------

void Material::setAmbient(const Color::sptr& ambient)
{
    m_ambient = ambient;
}

//------------------------------------------------------------------------------

void Material::setDiffuse(const Color::sptr& diffuse)
{
    m_diffuse = diffuse;
}

//------------------------------------------------------------------------------

void Material::setDiffuseTexture(const Image::sptr& diffuseTexture, const std::string& name)
{
    DiffuseTextureMap::const_iterator it = m_diffuseTexture.find(name);
    if(it != m_diffuseTexture.end())
    {
        it->second->setImage(diffuseTexture);
    }
    else
    {
        ::fwData::DiffuseTexture::sptr t = ::fwData::DiffuseTexture::New();
        t->setImage(diffuseTexture);

        m_diffuseTexture[name] = t;
    }
}

//------------------------------------------------------------------------------

Material::DiffuseTextureMap::const_iterator Material::getDiffuseTextureIteratorBegin() const
{
    return m_diffuseTexture.begin();
}

//------------------------------------------------------------------------------

Material::DiffuseTextureMap::const_iterator Material::getDiffuseTextureIteratorEnd() const
{
    return m_diffuseTexture.end();
}

//------------------------------------------------------------------------------

} //namespace fwData
