/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwDataCamp/Material.hpp"

#include <fwCamp/UserObject.hpp>

fwCampImplementDataMacro((fwData)(DiffuseTexture))
{
    builder
    .tag("object_version", "1")
    .tag("lib_name", "fwData")
    .base< ::fwData::Object>()
    ;
}

fwCampImplementEnumMacro((fwData)(DiffuseTexture)(FilteringType))
{
    builder
    .value("NEAREST", ::fwData::DiffuseTexture::NEAREST)
    .value("LINEAR", ::fwData::DiffuseTexture::LINEAR)
    ;
}

fwCampImplementEnumMacro((fwData)(DiffuseTexture)(WrappingType))
{
    builder
    .value("CLAMP", ::fwData::DiffuseTexture::CLAMP)
    .value("REPEAT", ::fwData::DiffuseTexture::REPEAT)
    ;
}

fwCampImplementEnumMacro((fwData)(DiffuseTexture)(BlendingType))
{
    builder
    .value("NONE", ::fwData::DiffuseTexture::NONE)
    .value("REPLACE", ::fwData::DiffuseTexture::REPLACE)
    .value("MODULATE", ::fwData::DiffuseTexture::MODULATE)
    .value("ADD", ::fwData::DiffuseTexture::ADD)
    .value("ADD_SIGNED", ::fwData::DiffuseTexture::ADD_SIGNED)
    .value("INTERPOLATE", ::fwData::DiffuseTexture::INTERPOLATE)
    .value("SUBTRACT", ::fwData::DiffuseTexture::SUBTRACT)
    ;
}
