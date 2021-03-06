/************************************************************************
 *
 * Copyright (C) 2014-2018 IRCAD France
 * Copyright (C) 2014-2018 IHU Strasbourg
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

#include "visuOgre/config.hpp"

#include <fwRuntime/Plugin.hpp>

#include <OgreLog.h>

namespace visuOgre
{
/**
 * @brief   This class is started when the bundles is loaded.
 */
class VISUOGRE_CLASS_API Plugin : public ::fwRuntime::Plugin
{
public:
    /// Destructor
    ~Plugin() noexcept;

    // Overrides
    VISUOGRE_API void start();

    // Overrides
    VISUOGRE_API void stop() noexcept;
};

/**
 * @brief Allows to redirect Ogre logs on Sight logs
 *        We need this class to be declared outside to export DLL symbols on Windows.
 */
class VISUOGRE_CLASS_API SightOgreListener : public ::Ogre::LogListener
{
public:
    /// Destructor, does nothing
    ~SightOgreListener()
    {
    }

    /**
     * @brief Set the message on Sight logs.
     *        Set Ogre log on Sight log depending on it's LogLevel.
     */
    VISUOGRE_API virtual void messageLogged(const ::Ogre::String& message, ::Ogre::LogMessageLevel lml,
                                            bool maskDebug, const ::Ogre::String& logName, bool& skipThisMessage);
};

} // namespace visuOgre
