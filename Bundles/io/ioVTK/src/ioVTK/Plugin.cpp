/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwRuntime/utils/GenericExecutableFactoryRegistrar.hpp>

#include "ioVTK/Plugin.hpp"
#include "ioVTK/SQImageReader.hpp"
namespace ioVTK
{

static ::fwRuntime::utils::GenericExecutableFactoryRegistrar<Plugin> registrar("::ioVTK::Plugin");

Plugin::~Plugin() noexcept
{
}

void Plugin::start()
{
    // Register a new reader of ::fwData::Image
    static const ::fwServices::QtQmlType<SQImageReader>    registar("com.fw4spl", 1, 0, "SImageReader");
}

void Plugin::stop() noexcept
{
}

} // namespace ioVTK
