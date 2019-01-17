/************************************************************************
 *
 * Copyright (C) 2009-2019 IRCAD France
 * Copyright (C) 2012-2019 IHU Strasbourg
 *
 * This file is part of Sight.
 *
 * Sight is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Sight. If not, see <https://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#include "visuOgreAdaptor/SDistance.hpp"

#include <fwData/Material.hpp>
#include <fwData/PointList.hpp>
#include <fwData/Reconstruction.hpp>

#include <fwDataTools/fieldHelper/Image.hpp>

#include <fwServices/macros.hpp>

fwServicesRegisterMacro( ::fwRenderOgre::IAdaptor, ::visuOgreAdaptor::SDistance, ::fwData::PointList );

namespace visuOgreAdaptor
{

SDistance::SDistance() noexcept
{

}

SDistance::~SDistance() noexcept
{

}

//------------------------------------------------------------------------------

void SDistance::setAxisColor( ::fwData::Color::sptr newColor) noexcept
{
    SLM_ASSERT("newColor not instanced", newColor);
    static_cast<void>(newColor);
}

//------------------------------------------------------------------------------

void SDistance::configuring()
{
}

//------------------------------------------------------------------------------

void SDistance::starting()
{
}

//------------------------------------------------------------------------------

void SDistance::updating()
{
}

//------------------------------------------------------------------------------

void SDistance::stopping()
{
}

}
