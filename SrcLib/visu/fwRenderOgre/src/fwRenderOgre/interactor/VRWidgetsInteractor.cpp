/************************************************************************
 *
 * Copyright (C) 2014-2018 IRCAD France
 * Copyright (C) 2014-2018 IHU Strasbourg
 *
 * This file is part of Sight.
 *
 * Sight is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Sight. If not, see <https://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#include "fwRenderOgre/interactor/VRWidgetsInteractor.hpp"

#include <fwRenderOgre/Layer.hpp>
#include <fwRenderOgre/registry/macros.hpp>

#include <fwCom/Signal.hxx>

#include <OGRE/OgreCamera.h>
#include <OGRE/OgreViewport.h>

fwRenderOgreRegisterInteractorMacro( ::fwRenderOgre::interactor::VRWidgetsInteractor );

namespace fwRenderOgre
{

namespace interactor
{

//------------------------------------------------------------------------------

VRWidgetsInteractor::VRWidgetsInteractor() noexcept :
    m_pickedObject(nullptr)
{
}

//------------------------------------------------------------------------------

VRWidgetsInteractor::~VRWidgetsInteractor() noexcept
{
}

//------------------------------------------------------------------------------

Ogre::MovableObject* VRWidgetsInteractor::pickObject(int x, int y)
{
    ::Ogre::Camera* camera = m_sceneManager->getCamera(::fwRenderOgre::Layer::DEFAULT_CAMERA_NAME);

    const int height = camera->getViewport()->getActualHeight();
    const int width  = camera->getViewport()->getActualWidth();

    const bool pickSuccess = m_picker.executeRaySceneQuery( x, y, width, height, 0 );

    return pickSuccess ? m_picker.getSelectedObject() : nullptr;
}

//------------------------------------------------------------------------------

void VRWidgetsInteractor::mouseMoveEvent(MouseButton button, int x, int y, int dx, int dy)
{
    auto widget = m_widget.lock();
    if(widget) // If a widget is present in the scene.
    {
        if(button == LEFT)
        {
            if(m_pickedObject == nullptr) // Enable trackball if no face was picked.
            {
                TrackballInteractor::mouseMoveEvent(button, x, y, dx, dy);
            }
            else
            {
                widget->widgetPicked(m_pickedObject, x, y);
            }
        }
        else if(button == MIDDLE)
        {
            widget->moveClippingBox(x, y, -dx, -dy);
        }
        else if(button == RIGHT)
        {
            widget->scaleClippingBox(x, y, dy);
        }
    }
    else // Fallback to trackball otherwise.
    {
        TrackballInteractor::mouseMoveEvent(button, x, y, dx, dy);
    }
}

//------------------------------------------------------------------------------

void VRWidgetsInteractor::buttonReleaseEvent(MouseButton /*button*/, int /*x*/, int /*y*/)
{
    auto widget = m_widget.lock();
    if(widget)
    {
        widget->widgetReleased();
        m_pickedObject = nullptr;
    }
}

//------------------------------------------------------------------------------

void VRWidgetsInteractor::buttonPressEvent(MouseButton button, int x, int y)
{
    auto widget = m_widget.lock();
    if(widget)
    {
        if(button == LEFT && widget->getVisibility())
        {
            m_pickedObject = pickObject(x, y);

            if(widget->belongsToWidget(m_pickedObject))
            {
                widget->widgetPicked(m_pickedObject, x, y);
            }
            else
            {
                m_pickedObject = nullptr;
            }
        }
        else if(button == MIDDLE)
        {
            widget->moveClippingBox(x, y, 0, 0);
        }
        else if(button == RIGHT)
        {
            widget->scaleClippingBox(x, y, 0);
        }
    }
}

//------------------------------------------------------------------------------

void VRWidgetsInteractor::setWidget(ui::VRWidget::sptr widget)
{
    m_widget = widget;
}

//------------------------------------------------------------------------------

void VRWidgetsInteractor::initPicker()
{
    m_picker.setSceneManager(this->m_sceneManager);
}

} // namespace interactor
} // namespace fwRenderOgre
