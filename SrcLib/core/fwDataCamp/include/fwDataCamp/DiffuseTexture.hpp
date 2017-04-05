/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDATACAMP_DIFFUSETEXTURE_HPP__
#define __FWDATACAMP_DIFFUSETEXTURE_HPP__

#include "fwDataCamp/config.hpp"

#include <fwCamp/macros.hpp>

#include <fwData/DiffuseTexture.hpp>

fwCampAutoDeclareEnumMacro((fwData)(DiffuseTexture)(FilteringType));
fwCampAutoDeclareEnumMacro((fwData)(DiffuseTexture)(WrappingType));
fwCampAutoDeclareEnumMacro((fwData)(DiffuseTexture)(BlendingType));

fwCampDeclareAccessor((fwData)(DiffuseTexture), (fwData)(Image));

#endif /* __FWDATACAMP_DIFFUSETEXTURE_HPP__ */
