/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwData/Texture.hpp"

#include "fwData/Exception.hpp"
#include "fwData/registry/macros.hpp"

fwDataRegisterMacro( ::fwData::Texture );

namespace fwData
{

Texture::Texture(::fwData::Object::Key key) :
    m_image(nullptr),
    m_filtering(::fwData::Texture::NEAREST),
    m_wrapping(::fwData::Texture::CLAMP),
    m_blending(::fwData::Texture::ADD)
{
}

Texture::~Texture()
{
}

//------------------------------------------------------------------------------

void Texture::shallowCopy(const Object::csptr& _source )
{
    Texture::csptr other = Texture::dynamicConstCast(_source);
    FW_RAISE_EXCEPTION_IF( ::fwData::Exception(
                               "Unable to copy" + (_source ? _source->getClassname() : std::string("<NULL>"))
                               + " to " + this->getClassname()), !bool(other) );
    this->fieldShallowCopy( _source );

    m_image     = other->m_image;
    m_filtering = other->m_filtering;
    m_wrapping  = other->m_wrapping;
    m_blending  = other->m_blending;
}

//------------------------------------------------------------------------------

void Texture::cachedDeepCopy(const Object::csptr& _source, DeepCopyCacheType& cache)
{
    Texture::csptr other = Texture::dynamicConstCast(_source);
    FW_RAISE_EXCEPTION_IF( ::fwData::Exception(
                               "Unable to copy" + (_source ? _source->getClassname() : std::string("<NULL>"))
                               + " to " + this->getClassname()), !bool(other) );
    this->fieldDeepCopy( _source );

    m_image     = ::fwData::Object::copy( other->m_image, cache );
    m_filtering = other->m_filtering;
    m_wrapping  = other->m_wrapping;
    m_blending  = other->m_blending;
}

//------------------------------------------------------------------------------

Image::sptr Texture::getImage() const
{
    return m_image;
}

//------------------------------------------------------------------------------

void Texture::setImage(const Image::sptr& texture)
{
    m_image = texture;
}

//------------------------------------------------------------------------------

const ::fwData::Texture::FilteringType Texture::getFiltering() const
{
    return m_filtering;
}

//------------------------------------------------------------------------------

void Texture::setFiltering(::fwData::Texture::FilteringType filtering)
{
    m_filtering = filtering;
}

//------------------------------------------------------------------------------

const ::fwData::Texture::WrappingType Texture::getWrapping() const
{
    return m_wrapping;
}

//------------------------------------------------------------------------------

void Texture::setWrapping(::fwData::Texture::WrappingType wrapping)
{
    m_wrapping = wrapping;
}

//------------------------------------------------------------------------------

::fwData::Texture::BlendingType Texture::getBlending() const
{
    return m_blending;
}

//------------------------------------------------------------------------------

void Texture::setBlending(::fwData::Texture::BlendingType blending)
{
    m_blending = blending;
}

//------------------------------------------------------------------------------

} //namespace fwData
