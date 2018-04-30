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
    virtual void FWRENDERVTK_API OpenGLInitState();
    virtual void FWRENDERVTK_API Render();
    void  FWRENDERVTK_API internalRender();
    void FWRENDERVTK_API setFrameBufferObject(QOpenGLFramebufferObject *);
    void FWRENDERVTK_API setRenderer(FrameBufferRenderer *);
    FrameBufferRenderer FWRENDERVTK_API *getRenderer() const;

private:
     FrameBufferRenderer *m_qtParentRenderer;
};

}



#endif // VTKINTERNALOPENGLRENDERWINDOW_HPP
