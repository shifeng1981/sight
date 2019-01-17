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

#pragma once

#include "visuOgreAdaptor/config.hpp"

#include <fwCom/Slot.hpp>
#include <fwCom/Slots.hpp>

#include <fwRenderOgre/IAdaptor.hpp>

class ogreCommand;

namespace visuOgreAdaptor
{

/**
 * @brief Adaptor to display distance on an image
 *
 * @section Slots Slots
 * - \b createDistance() : creates a new distance attached to this adaptor
 * - \b removeDistance(::fwData::PointList::sptr) : remove the distance
 *
 * @section XML XML Configuration
 *
 * @code{.xml}

   @endcode
 * @subsection In-Out In-Out
 * - \b image [::fwData::Image]: image containing the distance field.
 * @subsection Configuration Configuration
 **/
class SImageMultiDistances : public ::fwRenderOgre::IAdaptor
{
public:

    fwCoreServiceClassDefinitionsMacro( (SImageMultiDistances)(::fwRenderOgre::IAdaptor) )

    VISUOGREADAPTOR_API SImageMultiDistances() noexcept;

    VISUOGREADAPTOR_API ~SImageMultiDistances() noexcept;

    //VISUOGREADAPTOR_API void setNeedSubservicesDeletion(bool _needSubservicesDeletion);

    VISUOGREADAPTOR_API virtual void show(bool showDistances = true);
protected:
    VISUOGREADAPTOR_API void configuring() override;
    VISUOGREADAPTOR_API void starting() override;
    VISUOGREADAPTOR_API void updating() override;
    VISUOGREADAPTOR_API void stopping() override;
    bool m_needSubservicesDelection;
private:
    void removeDistance();

    void createDistance();
};
}
