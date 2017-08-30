/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWQTQUICK_RENDER_VTKQUICKITEM_HPP__
#define __FWQTQUICK_RENDER_VTKQUICKITEM_HPP__

#include "fwQtQuick/config.hpp"
#include "fwQtQuick/render/FboOffscreenWindow.hpp"
#include "fwQtQuick/utils/Register.hpp"

#include <QObject>
#include <QQuickFramebufferObject>
#include <QVTKInteractor.h>
#include <QVTKInteractorAdapter.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>

namespace fwQtQuick
{

namespace Render
{

class FWQTQUICK_API VtkQuickItem : public QQuickFramebufferObject
{
Q_OBJECT
public:
    // constructor
    VtkQuickItem();

    // destructor
    virtual ~VtkQuickItem();

    // Set the vtk render window, if you wish to use your own vtkRenderWindow
    void SetRenderWindow(vtkRenderWindow*);

    // method override to create renderer of the QuickFrameBufferObject
    Renderer* createRenderer() const override;

    // Get the vtk render window.
    vtkRenderWindow* GetRenderWindow();

    // Get the Qt/vtk interactor that was either created by default or set by the user
    QVTKInteractor* GetInteractor();

    // Called ONCE from the render thread before the FBO is first created and while the GUI thread is blocked
    void Init();

    static ::fwQtQuick::Register::Class<VtkQuickItem> Register;

    QMutex m_viewLock;

Q_SIGNALS:
    // Description:
    // This signal will be emitted whenever a mouse event occurs
    // within the QVTK window
    void mouseEvent(QMouseEvent* event);
private:
    // overloaded resize handler
    virtual void resizeEvent(QResizeEvent* event);
    // overloaded move handler
    virtual void moveEvent(QMoveEvent* event);

    // overloaded mouse press handler
    virtual void mousePressEvent(QMouseEvent* event);
    // overloaded mouse move handler
    virtual void mouseMoveEvent(QMouseEvent* event);
    // overloaded mouse release handler
    virtual void mouseReleaseEvent(QMouseEvent* event);
    // overloaded key press handler
    virtual void keyPressEvent(QKeyEvent* event);
    // overloaded key release handler
    virtual void keyReleaseEvent(QKeyEvent* event);
    // overloaded enter event
    virtual void enterEvent(QEvent*);
    // overloaded leave event
    virtual void leaveEvent(QEvent*);
    // overload wheel mouse event
    virtual void wheelEvent(QWheelEvent*);
    // overload focus event
    virtual void focusInEvent(QFocusEvent*);
    // overload focus event
    virtual void focusOutEvent(QFocusEvent*);
    // overload Qt's event() to capture more keys
    bool event( QEvent* e );
    // overload drag enter event
    virtual void dragEnterEvent(QDragEnterEvent*);
    // overload drag move event
    virtual void dragMoveEvent(QDragMoveEvent*);
    // overload drag leave event
    virtual void dragLeaveEvent(QDragLeaveEvent*);
    // overload drop event
    virtual void dropEvent(QDropEvent*);

    VtkQuickItem(VtkQuickItem const&)   = delete;
    void operator=(VtkQuickItem const&) = delete;

    FboOffscreenWindow* m_renderWindow;
    mutable FboRenderer* m_fboRenderer;

    QVTKInteractorAdapter* m_irenAdapter;

    vtkSmartPointer<vtkRenderWindowInteractor> m_interactor;
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> m_interactorStyle;

};
} // namespace render
} // namespace fwQtQuick
#endif //__FWQTQUICK_RENDER_VTKQUICKITEM_HPP__
