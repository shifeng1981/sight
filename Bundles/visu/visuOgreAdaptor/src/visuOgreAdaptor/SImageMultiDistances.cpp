/************************************************************************
 *
 * Copyright (C) 2019 IRCAD France
 * Copyright (C) 2019 IHU Strasbourg
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

#include "visuOgreAdaptor/SImageMultiDistances.hpp"

#include <fwData/Image.hpp>

#include <fwDataTools/fieldHelper/Image.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/op/Add.hpp>

fwServicesRegisterMacro( ::fwRenderOgre::IAdaptor, ::visuOgreAdaptor::SImageMultiDistances);

namespace visuOgreAdaptor
{

const ::fwServices::IService::KeyType s_IMAGE_INOUT = "image";

SImageMultiDistances::SImageMultiDistances() noexcept :
    m_needSubservicesDelection(false)
{

}

SImageMultiDistances::~SImageMultiDistances() noexcept
{

}

//------------------------------------------------------------------------------

void SImageMultiDistances::configuring()
{
}

//------------------------------------------------------------------------------

void SImageMultiDistances::starting()
{
}

//------------------------------------------------------------------------------

void SImageMultiDistances::updating()
{
}

//------------------------------------------------------------------------------

void SImageMultiDistances::stopping()
{
}

//------------------------------------------------------------------------------

void SImageMultiDistances::show(bool showDistances)
{
    if (showDistances)
    {
        this->starting();
    }
    else
    {
        this->stopping();
    }
}

//------------------------------------------------------------------------------

void SImageMultiDistances::removeDistance()
{
}

//------------------------------------------------------------------------------

void SImageMultiDistances::createDistance()
{
}

}
