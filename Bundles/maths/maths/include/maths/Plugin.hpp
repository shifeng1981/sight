/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2014-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "maths/config.hpp"

#include <fwRuntime/Plugin.hpp>

namespace maths
{

/**
 * @brief   This class is started when the bundle is loaded.
 */
struct MATHS_CLASS_API Plugin : public ::fwRuntime::Plugin
{
    /// Destructor. Does nothing.
    MATHS_API ~Plugin() noexcept;

    /// Overrides start method. Does nothing.
    MATHS_API void start();

    /// Overrides stop method. Does nothing
    MATHS_API void stop() noexcept;

};

} // namespace maths
