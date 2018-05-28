/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2015-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "appQml/config.hpp"

#include <fwRuntime/Plugin.hpp>

#include <fwServices/IQmlEngine.hpp>
#include <fwServices/QmlAppConfigManager.hpp>

namespace appQml
{

/**
 * @brief   This class is started when the bundles is loaded.
 */
class APPQML_CLASS_API Plugin : public ::fwRuntime::Plugin
{
public:
    /// Constructor.
    APPQML_API Plugin() noexcept;

    /// Destructor. Do nothing.
    APPQML_API ~Plugin() noexcept;

    /// Overrides start method. .
    APPQML_API void start();

    /// Overrides stop method. Do nothing
    APPQML_API void stop() noexcept;

    APPQML_API void initialize();

    APPQML_API void uninitialize() noexcept;

private:
    std::string m_configurationName;
    std::string m_parametersName;
    ::fwServices::QmlAppConfigManager::sptr m_appConfigMng;
    SPTR(::fwServices::IQmlEngine)  m_qmlEngine;
};

} // namespace appQml
