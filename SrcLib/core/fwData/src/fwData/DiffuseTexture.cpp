/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwData/DiffuseTexture.hpp"

#include "fwData/Exception.hpp"
#include "fwData/registry/macros.hpp"

fwDataRegisterMacro( ::fwData::DiffuseTexture );

namespace fwData
{

DiffuseTexture::DiffuseTexture(::fwData::Object::Key key) :
    m_image(nullptr),
    m_filtering(::fwData::DiffuseTexture::NEAREST),
    m_wrapping(::fwData::DiffuseTexture::CLAMP),
    m_blending(::fwData::DiffuseTexture::ADD)
{
}

DiffuseTexture::~DiffuseTexture()
{
}

//------------------------------------------------------------------------------

void DiffuseTexture::shallowCopy(const Object::csptr& _source )
{
    DiffuseTexture::csptr other = DiffuseTexture::dynamicConstCast(_source);
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

void DiffuseTexture::cachedDeepCopy(const Object::csptr& _source, DeepCopyCacheType& cache)
{
    DiffuseTexture::csptr other = DiffuseTexture::dynamicConstCast(_source);
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

Image::sptr DiffuseTexture::getImage() const
{
    return m_image;
}

//------------------------------------------------------------------------------

void DiffuseTexture::setImage(const Image::sptr& diffuseTexture)
{
    m_image = diffuseTexture;
}

//------------------------------------------------------------------------------

const ::fwData::DiffuseTexture::FilteringType DiffuseTexture::getFiltering() const
{
    return m_filtering;
}

//------------------------------------------------------------------------------

void DiffuseTexture::setFiltering(::fwData::DiffuseTexture::FilteringType filtering)
{
    m_filtering = filtering;
}

//------------------------------------------------------------------------------

const ::fwData::DiffuseTexture::WrappingType DiffuseTexture::getWrapping() const
{
    return m_wrapping;
}

//------------------------------------------------------------------------------

void DiffuseTexture::setWrapping(::fwData::DiffuseTexture::WrappingType wrapping)
{
    m_wrapping = wrapping;
}

//------------------------------------------------------------------------------

::fwData::DiffuseTexture::BlendingType DiffuseTexture::getBlending() const
{
    return m_blending;
}

//------------------------------------------------------------------------------

void DiffuseTexture::setBlending(::fwData::DiffuseTexture::BlendingType blending)
{
    m_blending = blending;
}

//------------------------------------------------------------------------------

} //namespace fwData
