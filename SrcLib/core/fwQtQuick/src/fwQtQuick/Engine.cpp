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

Engine::Engine() :
    m_rootObject(nullptr)
{
}

//-----------------------------------------------------------------------------

Engine::~Engine()
{
}

//-----------------------------------------------------------------------------

Engine* Engine::getInstance()
{
    static Engine instance;
    return &instance;
}

//-----------------------------------------------------------------------------

auto Engine::init()->void
{
    ::fwQtQuick::Register::Init();

    SLM_ASSERT("Quick frame is not instanced", this->getInstance());
    QQuickView* mainView = new QQuickView();
    auto engine          = mainView->engine();
    engine->addImportPath(m_RootPath.string().c_str());
    auto context = engine->rootContext();
    context->setContextProperty("App", this->getInstance());

    const auto rootQml = m_RootPath / "root.qml";
    mainView->setSource(QUrl::fromLocalFile(rootQml.string().c_str()));
    mainView->setPersistentOpenGLContext(true);
    mainView->setPersistentSceneGraph(true);
    mainView->setResizeMode(QQuickView::SizeRootObjectToView);
    mainView->show();

    m_rootObject = mainView->rootObject();
    ::fwQtQuick::code::Compiler::instance->openFile(m_FramePath.string().c_str());

}

//-----------------------------------------------------------------------------

auto Engine::getCompiler()->fwQtQuick::code::Compiler*
{
    return ::fwQtQuick::code::Compiler::instance;
}

//-----------------------------------------------------------------------------

} //namespace fwQtQuick
