/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWQTQUICK_RENDER_FBORENDERER_HPP__
#define __FWQTQUICK_RENDER_FBORENDERER_HPP__

#include "fwQtQuick/config.hpp"
#include "fwQtQuick/render/vtkQuickItem.hpp"

#include <QMouseEvent>
#include <QQuickFramebufferObject>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>

namespace fwQtQuick
{
namespace Render
{

class FboOffscreenWindow;

class FWQTQUICK_CLASS_API FboRenderer : public QQuickFramebufferObject::Renderer
{

friend class FboOffscreenWindow;

public:
    FWQTQUICK_API FboRenderer(FboOffscreenWindow*);
    FWQTQUICK_API void synchronize(QQuickFramebufferObject*) override;
    FWQTQUICK_API void render() override;
    FWQTQUICK_API QOpenGLFramebufferObject* createFramebufferObject(const QSize&) override;
    FWQTQUICK_API ~FboRenderer();

private:
    FboOffscreenWindow* m_fboOffscreenWindow;
    QOpenGLFramebufferObject* m_fbo;

    ::fwQtQuick::Render::VtkQuickItem* m_vtkQuickItem;

    bool m_readyToRender;
};
} // namespace render
} // namespace fwQtQuick

#endif // __FWQTQUICK_RENDER_FBORENDERER_HPP__
