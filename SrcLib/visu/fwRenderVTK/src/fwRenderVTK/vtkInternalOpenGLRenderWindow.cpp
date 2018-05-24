#include "fwRenderVTK/vtkInternalOpenGLRenderWindow.hpp"
#include "fwRenderVTK/FrameBufferItem.hpp"

#include <chrono>
#include <QQuickWindow>

namespace fwRenderVTK
{

vtkInternalOpenGLRenderWindow::vtkInternalOpenGLRenderWindow() :
    m_qtParentRenderer(0)
{
    this->OffScreenRenderingOn();
}

vtkInternalOpenGLRenderWindow::~vtkInternalOpenGLRenderWindow()
{
     this->OffScreenRendering = 0;
}

void    vtkInternalOpenGLRenderWindow::OpenGLInitState()
{
    this->MakeCurrent();
    initializeOpenGLFunctions();

    Superclass::OpenGLInitState();
    // Before any of the gl* functions in QOpenGLFunctions are called for a
    // given OpenGL context, an initialization must be run within that context
    glUseProgram(0); // Shouldn't Superclass::OpenGLInitState() handle this?
    glDisable(GL_DEPTH_TEST); // depth buffer fighting between the cone and the backround without this
    glDisable(GL_BLEND); // doesn't seem crucial (?) but it is one of the differnces that showed up in apitrace analysis
    GLfloat texcoords[] =
    {
        0,  0,
        0,  1,
        1,  1,
        1,  0
    };
    glTexCoordPointer(2, GL_FLOAT, 0, texcoords);

}

void    vtkInternalOpenGLRenderWindow::OpenGLEndState()
{
    glDepthMask(GL_TRUE);
}

void    vtkInternalOpenGLRenderWindow::internalRender()
{
    Superclass::Render();
}

void    vtkInternalOpenGLRenderWindow::Render()
{
    if (this->m_qtParentRenderer)
    {
        this->m_qtParentRenderer->update();
    }
}

void    vtkInternalOpenGLRenderWindow::setRenderer(FrameBufferRenderer *renderer)
{
    this->m_qtParentRenderer = renderer;
}

FrameBufferRenderer    *vtkInternalOpenGLRenderWindow::getRenderer() const
{
    return m_qtParentRenderer;
}

void    vtkInternalOpenGLRenderWindow::setFrameBufferObject(QOpenGLFramebufferObject *fbo)
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

vtkInternalOpenGLRenderWindow   *vtkInternalOpenGLRenderWindow::New()
{
    return new vtkInternalOpenGLRenderWindow;
}

}
