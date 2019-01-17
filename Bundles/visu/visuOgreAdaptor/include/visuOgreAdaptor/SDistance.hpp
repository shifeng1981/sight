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

#pragma once

#include "fwData/Color.hpp"
#include "fwData/Point.hpp"

#include "fwRenderOgre/IAdaptor.hpp"

#include "vector"

#include "visuOgreAdaptor/config.hpp"

namespace visuOgreAdaptor
{
/**
 * @brief Display the distance between the two points of the point list
 *
 * @section XML XML Configuration
 *
 * @code{.xml}
   @endcode
 * @subsection Input Input
 * - \b pointList [::fwData::PointList]: point list containing two points.
 * @subsection Configuration Configuration
 */
class SDistance : public ::fwRenderOgre::IAdaptor
{
public:
    fwCoreServiceClassDefinitionsMacro( (SDistance)(::fwRenderOgre::IAdaptor) )
    VISUOGREADAPTOR_API SDistance() noexcept;
    VISUOGREADAPTOR_API virtual ~SDistance() noexcept;

    static const ::fwServices::IService::KeyType s_POINTLIST_INPUT;

    /// set Distance Axis color AND alpha
    VISUOGREADAPTOR_API void setAxisColor( ::fwData::Color::sptr newColor) noexcept;
protected:
    VISUOGREADAPTOR_API void configuring() override;
    VISUOGREADAPTOR_API void starting() override;
    VISUOGREADAPTOR_API void updating() override;
    VISUOGREADAPTOR_API void stopping() override;
private:
    ::fwData::Point::cwptr m_point1;
    ::fwData::Point::cwptr m_point2;
};
}
