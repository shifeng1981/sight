#ifndef QVTKFRAMEBUFFER_H
#define QVTKFRAMEBUFFER_H

#include "fwRenderVTK/config.hpp"
#include "fwRenderVTK/vtkInternalOpenGLRenderWindow.hpp"

#include <fwServices/QtQmlType.hxx>

#include <vtkEventQtSlotConnect.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

#include <QVTKInteractorAdapter.h>

#include <QtQuick/QQuickFramebufferObject>
// Use the OpenGL API abstraction from Qt instead of from VTK because vtkgl.h
// and other Qt OpenGL-related headers do not play nice when included in the
// same compilation unit
#include <QOpenGLFunctions>
#include <qqmlapplicationengine.h>

namespace fwRenderVTK
{

class FrameBufferItem;

class  FWRENDERVTK_CLASS_API FrameBufferRenderer : public QObject,
                                                   public QQuickFramebufferObject::Renderer
{
    Q_OBJECT
public:
    FWRENDERVTK_API FrameBufferRenderer(vtkInternalOpenGLRenderWindow *, FrameBufferItem const*);
    FWRENDERVTK_API ~FrameBufferRenderer();
    /**
     * @brief createFramebufferObject: initialize a framebuffer
     * @param size: size of the framebuffer
     * @return QOpenGLFrameBufferObject: View where will be perform openGL command (VTK)
     */
    QOpenGLFramebufferObject FWRENDERVTK_API    *createFramebufferObject(const QSize& size);

    /**
     * @brief render: perform open GL command
     */
    void FWRENDERVTK_API   render();

    /**
     * @brief: synchronize m_item when frame is ready
     */
    virtual void FWRENDERVTK_API synchronize(QQuickFramebufferObject *);

    FrameBufferItem const   *getItem() const;

Q_SIGNALS:
    /**
     * @brief ready
     * Emit when frame is ready
     */
    void    ready();


private:
    vtkInternalOpenGLRenderWindow   *m_vtkRenderWindow;
    QOpenGLFramebufferObject    *m_framebufferObject;
    FrameBufferItem const *m_item;

    friend class vtkInternalOpenGLRenderWindow;
};

class FWRENDERVTK_CLASS_API  FrameBufferItem : public QQuickFramebufferObject
{
    Q_OBJECT

public:
    FWRENDERVTK_API FrameBufferItem(QQuickItem *parent = nullptr);
    FWRENDERVTK_API ~FrameBufferItem();

    /**
     *  @brief: create a FrameBufferRenderer instance
     */
    QQuickFramebufferObject::Renderer FWRENDERVTK_API  *createRenderer() const;
    /**
     *  @brief: return m_win
     */
    vtkInternalOpenGLRenderWindow    FWRENDERVTK_API *getRenderWindow() const;

    /**
     *  @brief: initialize variables
     */
    void FWRENDERVTK_API initialize();

    /**
     * @brief getRenderer
     * @return current renderer
     */
    vtkSmartPointer<vtkRenderer>    getRenderer() const;


protected:
    /**
     * Event management
     * Theses events are transmit to VTK by QVTKAdapter
     * @{
     */
    bool event(QEvent* evt) Q_DECL_OVERRIDE;

    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

    /**
     * }@
     */

Q_SIGNALS:
    /**
     * @brief ready: emit when FrameBufferItem is ready
     */
    void    ready();

private:
    vtkInternalOpenGLRenderWindow    *m_win;
    QVTKInteractorAdapter   *m_interactorAdapter;
    vtkSmartPointer<vtkRenderer>    m_renderer;
    vtkRenderWindowInteractor *m_interactor;

    friend class FrameBufferRenderer;
};

} // fwRenderVTK

#endif // QVTKFRAMEBUFFER_H
