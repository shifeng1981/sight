/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

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
#include <QMutex>

namespace fwRenderVTK
{

class FrameBufferItem;

class FWRENDERVTK_CLASS_QT_API FrameBufferRenderer : public QObject,
                                                     public QQuickFramebufferObject::Renderer
{
Q_OBJECT
public:
    FWRENDERVTK_QT_API FrameBufferRenderer(vtkInternalOpenGLRenderWindow*, FrameBufferItem*);
    FWRENDERVTK_QT_API ~FrameBufferRenderer();
    /**
     * @brief createFramebufferObject: initialize a framebuffer
     * @param size: size of the framebuffer
     * @return QOpenGLFrameBufferObject: View where will be perform openGL command (VTK)
     */
    QOpenGLFramebufferObject FWRENDERVTK_QT_API* createFramebufferObject(const QSize& size);

    /**
     * @brief render: perform open GL command
     */
    void FWRENDERVTK_QT_API   render();

    /**
     * @brief: synchronize m_item when frame is ready
     */
    virtual void FWRENDERVTK_QT_API synchronize(QQuickFramebufferObject*);

    FrameBufferItem const* getItem() const;

Q_SIGNALS:
    /**
     * @brief ready
     * Emit when frame is ready
     */
    void    ready();

private:
    vtkInternalOpenGLRenderWindow* m_vtkRenderWindow;
    QOpenGLFramebufferObject* m_framebufferObject;
    FrameBufferItem* m_item;
    bool m_readyToRender;

    friend class vtkInternalOpenGLRenderWindow;
};

class FWRENDERVTK_CLASS_QT_API FrameBufferItem : public QQuickFramebufferObject
{
Q_OBJECT

public:
    FWRENDERVTK_QT_API FrameBufferItem(QQuickItem* parent = nullptr);
    FWRENDERVTK_QT_API ~FrameBufferItem();

    /**
     *  @brief: create a FrameBufferRenderer instance
     */
    FWRENDERVTK_QT_API QQuickFramebufferObject::Renderer* createRenderer() const;
    /**
     *  @brief: return m_win
     */
    FWRENDERVTK_QT_API vtkInternalOpenGLRenderWindow* getRenderWindow() const;

    /**
     *  @brief: initialize variables
     */
    FWRENDERVTK_QT_API void  initialize();

    /**
     * @brief getRenderer
     * @return current renderer
     */
    FWRENDERVTK_QT_API vtkSmartPointer<vtkRenderer>    getRenderer() const;

    FWRENDERVTK_QT_API void    lockRenderer();
    FWRENDERVTK_QT_API void    unlockRenderer();

protected:
    /**
     * Event management
     * Theses events are transmit to VTK by QVTKAdapter
     * @{
     */
    FWRENDERVTK_QT_API bool event(QEvent* evt) Q_DECL_OVERRIDE;

    FWRENDERVTK_QT_API void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    FWRENDERVTK_QT_API void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    FWRENDERVTK_QT_API void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    FWRENDERVTK_QT_API void mouseDoubleClickEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

    /**
     * }@
     */

Q_SIGNALS:
    /**
     * @brief ready: emit when FrameBufferItem is ready
     */
    void    ready();

private:
    vtkInternalOpenGLRenderWindow* m_win;
    QVTKInteractorAdapter* m_interactorAdapter;
    vtkSmartPointer<vtkRenderer>    m_renderer;
    vtkRenderWindowInteractor* m_interactor;
    QMutex m_viewLock;

    friend class FrameBufferRenderer;
};

} // fwRenderVTK

#endif // QVTKFRAMEBUFFER_H
