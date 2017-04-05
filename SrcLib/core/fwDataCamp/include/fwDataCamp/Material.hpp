/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDATACAMP_MATERIAL_HPP__
#define __FWDATACAMP_MATERIAL_HPP__

#include "fwDataCamp/config.hpp"

#include <fwCamp/macros.hpp>

#include <fwData/Material.hpp>

fwCampAutoDeclareEnumMacro((fwData)(Material)(ShadingType));
fwCampAutoDeclareEnumMacro((fwData)(Material)(RepresentationType));
fwCampAutoDeclareEnumMacro((fwData)(Material)(OptionsType));

fwCampDeclareAccessor((fwData)(Material), (fwData)(DiffuseTexture));
fwCampDeclareAccessor((fwData)(Material), (fwData)(Color));
fwCampDeclareAccessor((fwData)(Material), (fwData)(Image));

#endif /* __FWDATACAMP_MATERIAL_HPP__ */
