/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "vtkSimpleNegato/Plugin.hpp"

#include <fwRuntime/utils/GenericExecutableFactoryRegistrar.hpp>

#include <fwServices/IQtQmlType.hpp>

namespace vtkSimpleNegato
{

static ::fwRuntime::utils::GenericExecutableFactoryRegistrar<Plugin> registrar("::vtkSimpleNegato::Plugin");

Plugin::~Plugin() noexcept
{
    ::fwServices::IQtQmlType::registarAllClasses();
}

//------------------------------------------------------------------------------

void Plugin::start()
{
}

//------------------------------------------------------------------------------

void Plugin::stop() noexcept
{
}

} // namespace vtkSimpleNegato
