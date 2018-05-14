/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "visuVTKAdaptor/Plugin.hpp"

#include <fwRuntime/utils/GenericExecutableFactoryRegistrar.hpp>
#include <visuVTKAdaptor/SQNegatoMPR.hpp>
#include <fwRenderVTK/FrameBufferItem.hpp>
#include <fwRenderVTK/SQRender.hpp>
#include <fwServices/macros.hpp>

namespace visuVTKAdaptor
{

static ::fwRuntime::utils::GenericExecutableFactoryRegistrar<Plugin> registrar("::visuVTKAdaptor::Plugin");

Plugin::~Plugin() noexcept
{
}

//------------------------------------------------------------------------------

void Plugin::start()
{
    const ::fwServices::QtQmlType<SQNegatoMPR>  registar("com.fw4spl.vtk.adaptors", 1, 0, "SNegatoMPR");

    ::fwServices::QtQmlType<::fwRenderVTK::FrameBufferItem>    registar2("com.fw4spl.vtk", 1, 0, "FrameBuffer");
    ::fwServices::QtQmlType<::fwRenderVTK::SQRender>   registar3("com.fw4spl.vtk", 1, 0, "SRender");
}

//------------------------------------------------------------------------------

void Plugin::stop() noexcept
{
}

} // namespace operators
