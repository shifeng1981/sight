/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwDataCamp/Material.hpp"

#include <fwCamp/UserObject.hpp>

fwCampImplementDataMacro((fwData)(Material))
{
    builder
    .tag("object_version", "5")
    .tag("lib_name", "fwData")
    .base< ::fwData::Object>()
    .property("shading_mode", &::fwData::Material::m_shadingMode)
    .property("representation_mode", &::fwData::Material::m_representationMode)
    .property("options_mode", &::fwData::Material::m_optionsMode)
    .property("ambient", &::fwData::Material::m_ambient)
    .property("diffuse", &::fwData::Material::m_diffuse)
    .property("texture", &::fwData::Material::m_texture)
    ;
}

fwCampImplementEnumMacro((fwData)(Material)(OptionsType))
{
    builder
    .value("STANDARD", ::fwData::Material::STANDARD)
    .value("NORMALS", ::fwData::Material::NORMALS)
    .value("CELLS_NORMALS", ::fwData::Material::CELLS_NORMALS)
    ;
}

fwCampImplementEnumMacro((fwData)(Material)(ShadingType))
{
    builder
    .value("AMBIENT", ::fwData::Material::AMBIENT)
    .value("FLAT", ::fwData::Material::FLAT)
    .value("GOURAUD", ::fwData::Material::GOURAUD)
    .value("PHONG", ::fwData::Material::PHONG)
    ;
}

fwCampImplementEnumMacro((fwData)(Material)(RepresentationType))
{
    builder
    .value("SURFACE",  ::fwData::Material::SURFACE)
    .value("POINT",  ::fwData::Material::POINT )
    .value("WIREFRAME",  ::fwData::Material::WIREFRAME)
    .value("EDGE",  ::fwData::Material::EDGE)
    ;
}

