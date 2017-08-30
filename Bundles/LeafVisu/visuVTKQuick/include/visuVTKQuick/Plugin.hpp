/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __VISUVTKQUICK_PLUGIN_HPP__
#define __VISUVTKQUICK_PLUGIN_HPP__

#include "visuVTKQuick/config.hpp"

#include <fwRuntime/Plugin.hpp>

namespace visuVTKQuick
{
/**
 * @brief   This class is started when the bundles is loaded.
 */
struct VISUVTKQUICK_CLASS_API Plugin : public ::fwRuntime::Plugin
{
    /**
     * @brief   destructor
     */
    ~Plugin() throw();

    // Overrides
    VISUVTKQUICK_API void start() throw(::fwRuntime::RuntimeException);

    // Overrides
    VISUVTKQUICK_API void stop() throw();

};

} // namespace visuVTKQuick

#endif //__VISUVTKQUICK_PLUGIN_HPP__
