/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "guiQtQuick/frame/QuickFrame.hpp"

#include <fwQtQuick/Engine.hpp>

#include <fwRuntime/Runtime.hpp>

#include <fwServices/macros.hpp>

#include <fwTools/fwID.hpp>

#include <QQuickView>

#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

#define ROOTPATH BUNDLE_PREFIX "/guiQtQuick_0-1"

namespace guiQtQuick
{
namespace frame
{

fwServicesRegisterMacro( ::fwGui::IFrameSrv, ::guiQtQuick::frame::QuickFrame, ::fwData::Object );

QuickFrame::QuickFrame() throw()
{
}

//-----------------------------------------------------------------------------

QuickFrame::~QuickFrame() throw()
{
}

//-----------------------------------------------------------------------------

void QuickFrame::configuring() throw( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();

    SLM_ASSERT( "<service> tag is required.", m_configuration->getName() == "service" );

    m_framePath = m_configuration->find("frame").at(0)->find("qml").at(0)->getValue();
}

//-----------------------------------------------------------------------------

void QuickFrame::starting() throw(::fwTools::Failed)
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

#ifdef ANDROID

    ::boost::filesystem::path qmlDir  = ::fwRuntime::Runtime::getDefault()->getWorkingPath() / "qml";
    ::boost::filesystem::path qmlDir2 = ::fwRuntime::Runtime::getDefault()->getWorkingPath() / "lib";
    QQmlEngine engine;
    engine.addImportPath(QString::fromStdString(qmlDir.c_str()));
    engine.addImportPath(QString::fromStdString(qmlDir2.c_str()));
#endif

    fwQtQuick::Engine* engine = fwQtQuick::Engine::getInstance();
    SLM_ASSERT(" fwQtQuickFrame is not instanced", engine);
    engine->setRootPath(ROOTPATH);
    engine->setFramePath(m_framePath.c_str());
    engine->init();

}

//-----------------------------------------------------------------------------

void QuickFrame::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void QuickFrame::info(std::ostream& _sstream )
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void QuickFrame::updating() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

}
}

