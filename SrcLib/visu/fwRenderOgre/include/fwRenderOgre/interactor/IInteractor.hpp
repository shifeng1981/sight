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

#pragma once

#include <fwRenderOgre/config.hpp>
#include <fwRenderOgre/factory/new.hpp>
#include <fwRenderOgre/registry/detail.hpp>

#include <fwCom/HasSignals.hpp>
#include <fwCom/Signal.hpp>
#include <fwCom/Signals.hpp>
#include <fwCom/Slot.hpp>
#include <fwCom/Slots.hpp>

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreVector3.h>

namespace fwRenderOgre
{

namespace interactor
{

/**
 * @brief Interface implementation for all the interaction with the mouse
 */
class FWRENDEROGRE_CLASS_API IInteractor
{

public:
    typedef std::weak_ptr< IInteractor > wptr;
    typedef std::shared_ptr< IInteractor > sptr;

    /// List of recognized mouse buttons.
    enum MouseButton
    {
        NONE,
        LEFT,
        MIDDLE,
        RIGHT,
        UNKNOWN
    };

    enum Modifier
    {
        SHIFT,
        CONTROL,
        META,
        ALT,
    };

    /**
     * @brief Class used to register a class factory in factory registry.
     * This class defines also the object factory ( 'create' )
     *
     * @tparam T Factory product type
     */
    template <typename T>
    class Registrar
    {
    public:
        Registrar(std::string functorKey)
        {
            ::fwRenderOgre::registry::getInteractorRegistry()->addFactory(functorKey,
                                                                          &::fwRenderOgre::interactorFactory::New<T>);
        }
    };

    /**
     * @name Signals API
     * @{
     */
    typedef ::fwCom::Signal< void () > RenderRequestedSigType;
    FWRENDEROGRE_API static const ::fwCom::Signals::SignalKeyType s_RENDER_REQUESTED_SIG;
    /** @} */

    /// Constructor. Retrieves the Ogre root and the \<sceneID\> scene manager
    FWRENDEROGRE_API IInteractor();

    /// Destructor
    FWRENDEROGRE_API virtual ~IInteractor();

    /// Change point of interest viewed by the camera
    FWRENDEROGRE_API void setSceneID(const std::string&);

    /// Behaviour on a MouseMoveEvent
    FWRENDEROGRE_API virtual void mouseMoveEvent(MouseButton, int, int, int, int) = 0;

    /// Behaviour on a WheelEvent
    FWRENDEROGRE_API virtual void wheelEvent(int, int, int) = 0;

    /// Called when the window is resized
    FWRENDEROGRE_API virtual void resizeEvent(int, int) = 0;

    /// Called when a key is pressed
    FWRENDEROGRE_API virtual void keyPressEvent(int) = 0;

    /// Called when a key is release
    FWRENDEROGRE_API virtual void keyReleaseEvent(int) = 0;

    /// Called when a mouse button is released.
    FWRENDEROGRE_API virtual void buttonReleaseEvent(MouseButton, int, int) = 0;

    /// Called when a mouse button is pressed.
    FWRENDEROGRE_API virtual void buttonPressEvent(MouseButton, int, int) = 0;

    /// Called when the focus is win
    FWRENDEROGRE_API virtual void focusInEvent() = 0;

    /// Called when the focus is lost
    FWRENDEROGRE_API virtual void focusOutEvent() = 0;

protected:

    /// Ogre Root
    ::Ogre::Root* m_ogreRoot;

    /// Current scene manager
    ::Ogre::SceneManager* m_sceneManager;

    /**
     * @name Signals attributes
     * @{
     */
    /// Signal triggered when a render is requested
    RenderRequestedSigType::sptr m_sigRenderRequested;
    /**
     * @}
     */
};
} // interactor
} // fwRenderOgre
