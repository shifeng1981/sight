/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwQtQuick/render/FboRenderer.hpp"

#include "fwQtQuick/render/FboOffscreenWindow.hpp"

#include <vtkCamera.h>
#include <vtkCommand.h>
#include <vtkRendererCollection.h>

namespace fwQtQuick
{
namespace Render
{

//------------------------------------------------------------------------------

FboRenderer::FboRenderer(FboOffscreenWindow* offscreenWindow) :
    m_fboOffscreenWindow(offscreenWindow),
    m_fbo(0),
    m_readyToRender(false)
{
    m_fboOffscreenWindow->Register(NULL);
    m_fboOffscreenWindow->QtParentRenderer = this;

}

//------------------------------------------------------------------------------

FboRenderer::~FboRenderer()
{
    m_fboOffscreenWindow->QtParentRenderer = 0;
    m_fboOffscreenWindow->Delete();
}

//------------------------------------------------------------------------------

void FboRenderer::synchronize(QQuickFramebufferObject* fbo)
{
    m_vtkQuickItem = static_cast< ::fwQtQuick::Render::VtkQuickItem*>(fbo);
    if (!m_readyToRender)
    {
        m_readyToRender = true;
        m_vtkQuickItem->Init();
//        m_vtkQuickItem->update();
    }
}

//------------------------------------------------------------------------------

void FboRenderer::render()
{
    if (!m_readyToRender)
    {
        return;
    }

    m_vtkQuickItem->m_viewLock.lock();
    m_fboOffscreenWindow->PushState();
    m_fboOffscreenWindow->OpenGLInitState();
    m_fboOffscreenWindow->InternalRender();
    m_fboOffscreenWindow->OpenGLEndState();
    m_fboOffscreenWindow->PopState();
    m_vtkQuickItem->m_viewLock.unlock();
}

//------------------------------------------------------------------------------

QOpenGLFramebufferObject* FboRenderer::createFramebufferObject(const QSize& size)
{
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::Depth);
    format.setTextureTarget(GL_TEXTURE_2D);
    format.setInternalTextureFormat(GL_RGBA32F_ARB);
    m_fbo = new QOpenGLFramebufferObject(size, format);
    m_fboOffscreenWindow->SetFramebufferObject(m_fbo);

    return m_fbo;
}

//------------------------------------------------------------------------------

} // namespace render
} // namespace fwQtQuick
