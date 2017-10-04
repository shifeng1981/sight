/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDATACAMP_TEXTURE_HPP__
#define __FWDATACAMP_TEXTURE_HPP__

#include "fwDataCamp/config.hpp"

#include <fwCamp/macros.hpp>

#include <fwData/Texture.hpp>

fwCampAutoDeclareEnumMacro((fwData)(Texture)(FilteringType));
fwCampAutoDeclareEnumMacro((fwData)(Texture)(WrappingType));
fwCampAutoDeclareEnumMacro((fwData)(Texture)(BlendingType));

fwCampDeclareAccessor((fwData)(Texture), (fwData)(Image));

#endif /* __FWDATACAMP_TEXTURE_HPP__ */
