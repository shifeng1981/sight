/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwQtQuick/render/vtkQuickItem.hpp"

#include "fwQtQuick/render/FboRenderer.hpp"

#include <fwCore/spyLog.hpp>

#include <QQuickItem>

#if SPYLOG_LEVEL >= 5
// For debug pupose
class customFilter : public QObject
{
public:
    customFilter()
    {
    }
protected:
    bool eventFilter(QObject* obj, QEvent* event);

};

//------------------------------------------------------------------------------

bool customFilter::eventFilter(QObject* obj, QEvent* event)
{
    OSLM_DEBUG(" event type = "<<event->type());
}
#endif
namespace fwQtQuick
{
namespace Render
{

::fwQtQuick::Register::Class<VtkQuickItem> VtkQuickItem::Register;

//-----------------------------------------------------------------------------

VtkQuickItem::VtkQuickItem()
{
    this->setAcceptedMouseButtons(Qt::AllButtons);
#if SPYLOG_LEVEL >= 5
    this->installEventFilter(new customFilter);
#endif
    this->setMirrorVertically(true); // else scene is verticaly flipped

    m_renderWindow = ::fwQtQuick::Render::FboOffscreenWindow::New();
    m_irenAdapter  = new QVTKInteractorAdapter(this);

    m_interactor      = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    m_interactorStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();

    m_interactor->SetRenderWindow(m_renderWindow);
    m_interactor->SetInteractorStyle(m_interactorStyle);
}

//-----------------------------------------------------------------------------

VtkQuickItem::~VtkQuickItem()
{
    this->SetRenderWindow(nullptr);
}

//-----------------------------------------------------------------------------

QQuickFramebufferObject::Renderer* VtkQuickItem::createRenderer() const
{
    m_fboRenderer =
        new ::fwQtQuick::Render::FboRenderer(static_cast< ::fwQtQuick::Render::FboOffscreenWindow*>(m_renderWindow));
    // hack... else segmentation fault in vtkOpenGLRenderWindow line 386:
    // initialize blending for transparency
    // glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
    //                     GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
    m_interactor->Initialize();
    return m_fboRenderer;
}

//-----------------------------------------------------------------------------

void VtkQuickItem::SetRenderWindow(vtkRenderWindow* renderWindow)
{
    if(m_renderWindow != renderWindow)
    {
        m_renderWindow = static_cast< ::fwQtQuick::Render::FboOffscreenWindow*>(renderWindow);
    }
}

//-----------------------------------------------------------------------------

vtkRenderWindow* VtkQuickItem::GetRenderWindow()
{
    return static_cast<vtkRenderWindow*>(m_renderWindow);
}

//-----------------------------------------------------------------------------

QVTKInteractor* VtkQuickItem::GetInteractor()
{
    return QVTKInteractor::SafeDownCast(m_interactor);
}

//-----------------------------------------------------------------------------

void VtkQuickItem::Init()
{
    m_renderWindow->OpenGLInitContext();
}

//-----------------------------------------------------------------------------

/*! overloaded Qt's event handler to capture additional keys that Qt has
   default behavior for (for example the Tab and Shift-Tab key)
 */
bool VtkQuickItem::event(QEvent* e)
{
    SLM_TRACE_FUNC();
    if(e->type() == QEvent::TouchBegin ||
       e->type() == QEvent::TouchUpdate ||
       e->type() == QEvent::TouchEnd)
    {
        if(this->m_renderWindow)
        {
            this->m_viewLock.lock();
            m_irenAdapter->ProcessEvent(e, m_interactor);
            this->m_viewLock.unlock();
            if (e->isAccepted())
            {
                return true;
            }
        }
    }

    if(QQuickItem::event(e))
    {
        return true;
    }

    if(e->type() == QEvent::KeyPress)
    {
        QKeyEvent* ke = static_cast<QKeyEvent*>(e);
        this->keyPressEvent(ke);
        return ke->isAccepted();
    }

    return QQuickFramebufferObject::event(e);
}

//------------------------------------------------------------------------------

/*! handle resize event
 */
void VtkQuickItem::resizeEvent(QResizeEvent* e)
{
    SLM_TRACE_FUNC();
    if(!this->m_renderWindow)
    {
        return;
    }
    // Don't set size on subclass of vtkRenderWindow or it triggers recursion.
    // Getting this event in the first place means the window was already
    // resized and we're updating the sizes in VTK.
    this->m_renderWindow->vtkRenderWindow::SetSize(this->width(), this->height());

    // and update the interactor
    if(this->m_renderWindow->GetInteractor())
    {
        this->m_viewLock.lock();
        m_irenAdapter->ProcessEvent(e, m_interactor);
        this->m_viewLock.unlock();
    }

}

//------------------------------------------------------------------------------

void VtkQuickItem::moveEvent(QMoveEvent* e)
{
    SLM_TRACE_FUNC();
    if(!this->m_renderWindow)
    {
        return;
    }

    // Don't set size on subclass of vtkRenderWindow or it triggers recursion.
    // Getting this event in the first place means the window was already
    // resized and we're updating the sizes in VTK.
    this->m_renderWindow->SetPosition(this->x(), this->y());
}

//------------------------------------------------------------------------------

/*! handle mouse press event
 */
void VtkQuickItem::mousePressEvent(QMouseEvent* e)
{
    SLM_TRACE_FUNC();
    // Emit a mouse press event for anyone who might be interested
    Q_EMIT mouseEvent(e);

    if(this->m_renderWindow)
    {
        this->m_viewLock.lock();
        m_irenAdapter->ProcessEvent(e, m_interactor);
        this->m_viewLock.unlock();
    }

}

//------------------------------------------------------------------------------

/*! handle mouse move event
 */
void VtkQuickItem::mouseMoveEvent(QMouseEvent* e)
{

    SLM_TRACE_FUNC();
    if(this->m_renderWindow)
    {
        this->m_viewLock.lock();
        m_irenAdapter->ProcessEvent(e, m_interactor);
        this->m_viewLock.unlock();

        // Emit a mouse press event for anyone who might be interested
        Q_EMIT mouseEvent(e);
    }
}

//------------------------------------------------------------------------------

/*! handle enter event
 */
void VtkQuickItem::enterEvent(QEvent* e)
{
    SLM_TRACE_FUNC();
    if(this->m_renderWindow)
    {
        this->m_viewLock.lock();
        m_irenAdapter->ProcessEvent(e, m_interactor);
        this->m_viewLock.unlock();
    }
}

//------------------------------------------------------------------------------

/*! handle leave event
 */
void VtkQuickItem::leaveEvent(QEvent* e)
{
    SLM_TRACE_FUNC();
    if(this->m_renderWindow)
    {
        this->m_viewLock.lock();
        m_irenAdapter->ProcessEvent(e, m_interactor);
        this->m_viewLock.unlock();
    }
}

//------------------------------------------------------------------------------

/*! handle mouse release event
 */
void VtkQuickItem::mouseReleaseEvent(QMouseEvent* e)
{
    SLM_TRACE_FUNC();
    if(this->m_renderWindow)
    {
        this->m_viewLock.lock();
        m_irenAdapter->ProcessEvent(e, m_interactor);
        this->m_viewLock.unlock();

        // Emit a mouse press event for anyone who might be interested
        Q_EMIT mouseEvent(e);
    }
}

//------------------------------------------------------------------------------

/*! handle key press event
 */
void VtkQuickItem::keyPressEvent(QKeyEvent* e)
{
    SLM_TRACE_FUNC();
    if(this->m_renderWindow)
    {
        this->m_viewLock.lock();
        m_irenAdapter->ProcessEvent(e, m_interactor);
        this->m_viewLock.unlock();
    }
}

//------------------------------------------------------------------------------

/*! handle key release event
 */
void VtkQuickItem::keyReleaseEvent(QKeyEvent* e)
{
    SLM_TRACE_FUNC();
    if(this->m_renderWindow)
    {
        this->m_viewLock.lock();
        m_irenAdapter->ProcessEvent(e, m_interactor);
        this->m_viewLock.unlock();
    }
}

//------------------------------------------------------------------------------

void VtkQuickItem::wheelEvent(QWheelEvent* e)
{
    SLM_TRACE_FUNC();
    if(this->m_renderWindow)
    {
        this->m_viewLock.lock();
        m_irenAdapter->ProcessEvent(e, m_interactor);
        this->m_viewLock.unlock();
    }
}

//------------------------------------------------------------------------------

void VtkQuickItem::focusInEvent(QFocusEvent* e)
{
    SLM_TRACE_FUNC();
    // These prevent updates when the window
    // gains or loses focus.  By default, Qt
    // does an update because the color group's
    // active status changes.  We don't even use
    // color groups so we do nothing here.

    // also pass to interactor
    this->m_viewLock.lock();
    m_irenAdapter->ProcessEvent(e, m_interactor);
    this->m_viewLock.unlock();
}

//------------------------------------------------------------------------------

void VtkQuickItem::focusOutEvent(QFocusEvent* e)
{
    SLM_TRACE_FUNC();
    // These prevent updates when the window
    // gains or loses focus.  By default, Qt
    // does an update because the color group's
    // active status changes.  We don't even use
    // color groups so we do nothing here.

    // also pass to interactor
    this->m_viewLock.lock();
    m_irenAdapter->ProcessEvent(e, m_interactor);
    this->m_viewLock.unlock();
}

//------------------------------------------------------------------------------

void VtkQuickItem::dragEnterEvent(QDragEnterEvent* e)
{
    SLM_TRACE_FUNC();
    if(this->m_renderWindow)
    {
        this->m_viewLock.lock();
        m_irenAdapter->ProcessEvent(e, m_interactor);
        this->m_viewLock.unlock();
    }
}

//------------------------------------------------------------------------------

void VtkQuickItem::dragMoveEvent(QDragMoveEvent* e)
{
    SLM_TRACE_FUNC();
    if(this->m_renderWindow)
    {
        this->m_viewLock.lock();
        m_irenAdapter->ProcessEvent(e, m_interactor);
        this->m_viewLock.unlock();
    }
}

//------------------------------------------------------------------------------

void VtkQuickItem::dragLeaveEvent(QDragLeaveEvent* e)
{
    SLM_TRACE_FUNC();
    if(this->m_renderWindow)
    {
        this->m_viewLock.lock();
        m_irenAdapter->ProcessEvent(e, m_interactor);
        this->m_viewLock.unlock();
    }
}

//------------------------------------------------------------------------------

void VtkQuickItem::dropEvent(QDropEvent* e)
{
    SLM_TRACE_FUNC();
    if(this->m_renderWindow)
    {
        this->m_viewLock.lock();
        m_irenAdapter->ProcessEvent(e, m_interactor);
        this->m_viewLock.unlock();
    }
}

//------------------------------------------------------------------------------

} // namespace render
} // namespace fwQtQuick
