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

    m_ambient = other->m_ambient;
    m_diffuse = other->m_diffuse;
    m_texture = other->m_texture;

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

    m_ambient = ::fwData::Object::copy( other->m_ambient, cache );
    m_diffuse = ::fwData::Object::copy( other->m_diffuse, cache );

    TextureMap::const_iterator it = other->getTextureIteratorBegin();
    for(; it != other->getTextureIteratorEnd(); it++)
    {
        ::fwData::Texture::sptr dt = ::fwData::Texture::New();
        dt->deepCopy(it->second);
        m_texture[it->first] = dt;
    }

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

Image::sptr Material::getTexture(const std::string& name) const
{
    TextureMap::const_iterator it = m_texture.find(name);
    if(it != m_texture.end())
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

void Material::setTexture(const Image::sptr& texture, const std::string& name)
{
    TextureMap::const_iterator it = m_texture.find(name);
    if(it != m_texture.end())
    {
        it->second->setImage(texture);
    }
    else
    {
        ::fwData::Texture::sptr t = ::fwData::Texture::New();
        t->setImage(texture);

        m_texture[name] = t;
    }
}

//------------------------------------------------------------------------------

Material::TextureMap::const_iterator Material::getTextureIteratorBegin() const
{
    return m_texture.begin();
}

//------------------------------------------------------------------------------

Material::TextureMap::const_iterator Material::getTextureIteratorEnd() const
{
    return m_texture.end();
}

//------------------------------------------------------------------------------

} //namespace fwData
