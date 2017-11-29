/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "guiQtQuick/frame/QuickFrame.hpp"

#include <fwQtQuick/Engine.hpp>

#include <fwRuntime/operations.hpp>
#include <fwRuntime/Runtime.hpp>

#include <fwServices/macros.hpp>

#include <fwTools/fwID.hpp>

#include <QQuickView>

#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

namespace guiQtQuick
{
namespace frame
{

fwServicesRegisterMacro( ::fwGui::IFrameSrv, ::guiQtQuick::frame::QuickFrame);

QuickFrame::QuickFrame()
{
}

//-----------------------------------------------------------------------------

QuickFrame::~QuickFrame()
{
}

//-----------------------------------------------------------------------------

void QuickFrame::configuring()
{
    SLM_ASSERT( "<service> tag is required.", m_configuration->getName() == "service" );

    m_framePath = m_configuration->find("frame").at(0)->find("qml").at(0)->getValue();
}

//-----------------------------------------------------------------------------

void QuickFrame::starting()
{
#if SPYLOG_LEVEL >= 5

    const char* qmlEnvTraceName  = "QML_IMPORT_TRACE";
    const char* qmlEnvTraceValue = "1";
#ifdef WIN32
    size_t envsize = 0;
    int errcode    = getenv_s(&envsize, NULL, 0, qmlEnvTraceName);
    if(errcode || envsize)
    {
        OSLM_WARN("Environment variable '"<<qmlEnvTraceName<<"' already defined.");
    }
    else
    {
        _putenv_s(qmlEnvTraceName, qmlEnvTraceValue);
    }
#else
    setenv(qmlEnvTraceName, qmlEnvTraceValue, false); // useful to debug qml
#endif

#endif

    ::fwQtQuick::Engine* engine = ::fwQtQuick::Engine::getInstance();
    SLM_ASSERT(" fwQtQuickFrame is not instanced", engine);
    const auto bundlePath = ::fwRuntime::getBundleResourcePath(std::string("guiQtQuick"));
    engine->setRootPath(bundlePath);
    const auto path = ::fwRuntime::getBundleResourceFilePath(m_framePath);
    engine->setFramePath(path);
    engine->init();
}

//-----------------------------------------------------------------------------

void QuickFrame::stopping()
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void QuickFrame::updating()
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

}
}

