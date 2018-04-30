#ifndef VTKINTERNALOPENGLRENDERWINDOW_HPP
#define VTKINTERNALOPENGLRENDERWINDOW_HPP

# include "fwRenderVTK/config.hpp"

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkExternalOpenGLRenderWindow.h>
#include <vtkObjectFactory.h>
#include <vtkRendererCollection.h>
#include <vtkCamera.h>

#include <QOpenGLFunctions_2_0>
#include <QQuickFramebufferObject>
#include <QOpenGLFramebufferObject>


namespace fwRenderVTK
{

class FrameBufferRenderer;


/**
 * @brief The vtkInternalOpenGLRenderWindow class
 *
 * This class is used to render a offscreen VTK window in a FrameBuffer
 */
class FWRENDERVTK_CLASS_API   vtkInternalOpenGLRenderWindow : public vtkExternalOpenGLRenderWindow, protected QOpenGLFunctions_2_0
{
public:
    friend class FrameBufferRenderer;

    static vtkInternalOpenGLRenderWindow FWRENDERVTK_API    *New();
    vtkTypeMacro(vtkInternalOpenGLRenderWindow, vtkGenericOpenGLRenderWindow);


protected:
    FWRENDERVTK_API vtkInternalOpenGLRenderWindow();
    FWRENDERVTK_API ~vtkInternalOpenGLRenderWindow();

public:
    /**
     *  @brief: initialize openGL context/functions
     */
    virtual void FWRENDERVTK_API OpenGLInitState();

    /**
     * @brief: Call render method of the window
     */
    virtual void FWRENDERVTK_API Render();
    /**
     *  @brief: render into the framebuffer
     */
    void  FWRENDERVTK_API internalRender();
    /**
     *  @brief: set frame buffer to the window
     */
    void FWRENDERVTK_API setFrameBufferObject(QOpenGLFramebufferObject *);
    /**
     * @brief: configure renderer
     */
    void FWRENDERVTK_API setRenderer(FrameBufferRenderer *);
    /**
     * @brief: return renderer
     */
    FrameBufferRenderer FWRENDERVTK_API *getRenderer() const;

private:
     FrameBufferRenderer *m_qtParentRenderer;
};

}



#endif // VTKINTERNALOPENGLRENDERWINDOW_HPP
