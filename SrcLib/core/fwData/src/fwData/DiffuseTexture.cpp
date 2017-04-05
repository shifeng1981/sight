/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwData/DiffuseTexture.hpp"

#include "fwData/Exception.hpp"
#include "fwData/registry/macros.hpp"

#include <fwCom/Signal.hpp>
#include <fwCom/Signal.hxx>

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
}

//------------------------------------------------------------------------------

void DiffuseTexture::cachedDeepCopy(const Object::csptr& _source, DeepCopyCacheType& cache)
{
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

const ::fwData::DiffuseTexture::FilteringType DiffuseTexture::getFiltering()
{
    return m_filtering;
}

//------------------------------------------------------------------------------

::fwData::DiffuseTexture::FilteringType& DiffuseTexture::getRefFiltering()
{
    return m_filtering;
}

//------------------------------------------------------------------------------

const ::fwData::DiffuseTexture::FilteringType& DiffuseTexture::getCRefFiltering() const
{
    return m_filtering;
}

//------------------------------------------------------------------------------

void DiffuseTexture::setFiltering(::fwData::DiffuseTexture::FilteringType filtering)
{
    m_filtering = filtering;
}

//------------------------------------------------------------------------------

const ::fwData::DiffuseTexture::WrappingType DiffuseTexture::getWrapping()
{
    return m_wrapping;
}

//------------------------------------------------------------------------------

::fwData::DiffuseTexture::WrappingType & DiffuseTexture::getRefWrapping()
{
    return m_wrapping;
}

//------------------------------------------------------------------------------

const ::fwData::DiffuseTexture::WrappingType& DiffuseTexture::getCRefWrapping() const
{
    return m_wrapping;
}

//------------------------------------------------------------------------------

void DiffuseTexture::setWrapping(::fwData::DiffuseTexture::WrappingType wrapping)
{
    m_wrapping = wrapping;
}

//------------------------------------------------------------------------------

::fwData::DiffuseTexture::BlendingType DiffuseTexture::getBlending()
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
