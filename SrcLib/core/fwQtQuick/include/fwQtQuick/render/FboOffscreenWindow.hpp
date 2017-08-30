/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWQTQUICK_RENDER_FBOOFFSCREENWINDOW_HPP__
#define __FWQTQUICK_RENDER_FBOOFFSCREENWINDOW_HPP__

#include "fwQtQuick/config.hpp"

#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions>
#include <vtkExternalOpenGLRenderWindow.h>

namespace fwQtQuick
{
namespace Render
{

class FboRenderer;

class FWQTQUICK_CLASS_API FboOffscreenWindow :
    public vtkExternalOpenGLRenderWindow,
    protected QOpenGLFunctions
{
public:
    FboRenderer* QtParentRenderer;
    FWQTQUICK_API static FboOffscreenWindow* New();
    FWQTQUICK_API virtual void OpenGLInitState() override;
    FWQTQUICK_API void OpenGLEndState();
    FWQTQUICK_API void Render() override;
    FWQTQUICK_API void InternalRender();
    FWQTQUICK_API void SetFramebufferObject(QOpenGLFramebufferObject*);

protected:
    FboOffscreenWindow();
    ~FboOffscreenWindow();
};
} // namespace render
} // namespace fwQtQuick

#endif // __FWQTQUICK_RENDER_FBOOFFSCREENWINDOW_HPP__
