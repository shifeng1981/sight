/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwQtQuick/render/FboOffscreenWindow.hpp"

#include "fwQtQuick/render/FboRenderer.hpp"

namespace fwQtQuick
{

namespace Render
{

//------------------------------------------------------------------------------

FboOffscreenWindow::FboOffscreenWindow() :
    QtParentRenderer(0)
{
    this->OffScreenRenderingOn();
}

//------------------------------------------------------------------------------

FboOffscreenWindow::~FboOffscreenWindow()
{
    this->OffScreenRendering = 0;
}

//------------------------------------------------------------------------------

FboOffscreenWindow* FboOffscreenWindow::New()
{
    return new FboOffscreenWindow();
}

//------------------------------------------------------------------------------

void FboOffscreenWindow::OpenGLInitState()
{
    this->MakeCurrent();
    initializeOpenGLFunctions();

    Superclass::OpenGLInitState();
    glUseProgram(0);

    glEnable(GL_BLEND);
    glHint(GL_CLIP_VOLUME_CLIPPING_HINT_EXT, GL_FASTEST);
    glDepthMask(GL_TRUE);
}

//------------------------------------------------------------------------------

void FboOffscreenWindow::OpenGLEndState()
{
    glDepthMask(GL_TRUE);
}

//------------------------------------------------------------------------------

void FboOffscreenWindow::Render()
{
    if (this->QtParentRenderer)
    {
        this->QtParentRenderer->update();
    }
}

//------------------------------------------------------------------------------

void FboOffscreenWindow::InternalRender()
{
    Superclass::Render();
}

//------------------------------------------------------------------------------

void FboOffscreenWindow::SetFramebufferObject(QOpenGLFramebufferObject* fbo)
{
    this->SetFrontBuffer(GL_COLOR_ATTACHMENT0);
    this->SetFrontRightBuffer(GL_COLOR_ATTACHMENT0);
    this->SetBackLeftBuffer(GL_COLOR_ATTACHMENT0);
    this->SetBackRightBuffer(GL_COLOR_ATTACHMENT0);

    auto size = fbo->size();

    this->Size[0]                 = size.width();
    this->Size[1]                 = size.height();
    this->NumberOfFrameBuffers    = 1;
    this->FrameBufferObject       = static_cast<unsigned int>(fbo->handle());
    this->DepthRenderBufferObject = 0;
    this->TextureObjects[0]       = static_cast<unsigned int>(fbo->texture());
    this->OffScreenRendering      = true;
    this->OffScreenUseFrameBuffer = true;
    this->Modified();
}

//------------------------------------------------------------------------------

} // namespace render
} // namespace fwQtQuick
