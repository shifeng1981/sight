/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwQtQuick/Engine.hpp"

#include <fwCore/spyLog.hpp>

namespace fwQtQuick
{

//-----------------------------------------------------------------------------

Engine::Engine()
{
    //----
}

//-----------------------------------------------------------------------------

Engine::~Engine()
{
    //----
}

//-----------------------------------------------------------------------------

Engine* Engine::getInstance()
{
    SLM_TRACE_FUNC();
    static Engine instance;
    return &instance;
}

//-----------------------------------------------------------------------------

auto Engine::init()->void
{
    SLM_TRACE_FUNC();
    fwQtQuick::Register::Init();
    SLM_ASSERT("Quick frame is not instancied", this->getInstance());
    QQuickView* mainView = new QQuickView();
    auto engine          = mainView->engine();
    engine->addImportPath(m_RootPath.c_str());
    auto context = engine->rootContext();
    context->setContextProperty("App", this->getInstance());

    std::string rootQml = m_RootPath + std::string("/root.qml");
    mainView->setSource(QUrl(rootQml.c_str()));
    mainView->setPersistentOpenGLContext(true);
    mainView->setPersistentSceneGraph(true);
    mainView->setResizeMode(QQuickView::SizeRootObjectToView);
    mainView->show();

    m_rootObject = mainView->rootObject();

    fwQtQuick::code::Compiler::instance->openFile(QString(m_FramePath.c_str()));

}

//-----------------------------------------------------------------------------

auto Engine::getCompiler()->fwQtQuick::code::Compiler*
{
    SLM_TRACE_FUNC();
    return fwQtQuick::code::Compiler::instance;
}

//-----------------------------------------------------------------------------

} //namespace fwQtQuick
