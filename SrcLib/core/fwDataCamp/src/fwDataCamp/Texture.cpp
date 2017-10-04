/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwDataCamp/Texture.hpp"

#include <fwCamp/UserObject.hpp>

fwCampImplementDataMacro((fwData)(Texture))
{
    builder
    .tag("object_version", "1")
    .tag("lib_name", "fwData")
    .base< ::fwData::Object>()
    .property("image", &::fwData::Texture::m_image)
    .property("filtering", &::fwData::Texture::m_filtering)
    .property("wrapping", &::fwData::Texture::m_wrapping)
    .property("blending", &::fwData::Texture::m_blending)
    ;
}

fwCampImplementEnumMacro((fwData)(Texture)(FilteringType))
{
    builder
    .value("NEAREST", ::fwData::Texture::NEAREST)
    .value("LINEAR", ::fwData::Texture::LINEAR)
    ;
}

fwCampImplementEnumMacro((fwData)(Texture)(WrappingType))
{
    builder
    .value("CLAMP", ::fwData::Texture::CLAMP)
    .value("REPEAT", ::fwData::Texture::REPEAT)
    ;
}

fwCampImplementEnumMacro((fwData)(Texture)(BlendingType))
{
    builder
    .value("NONE", ::fwData::Texture::NONE)
    .value("REPLACE", ::fwData::Texture::REPLACE)
    .value("MODULATE", ::fwData::Texture::MODULATE)
    .value("ADD", ::fwData::Texture::ADD)
    .value("ADD_SIGNED", ::fwData::Texture::ADD_SIGNED)
    .value("INTERPOLATE", ::fwData::Texture::INTERPOLATE)
    .value("SUBTRACT", ::fwData::Texture::SUBTRACT)
    ;
}
