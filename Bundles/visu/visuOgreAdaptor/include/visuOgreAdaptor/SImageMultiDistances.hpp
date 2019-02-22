/************************************************************************
 *
 * Copyright (C) 2019 IRCAD France
 * Copyright (C) 2019 IHU Strasbourg
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

#include "visuOgreAdaptor/config.hpp"
#include "visuOgreAdaptor/SLine.hpp"

#include <fwData/PointList.hpp>
#include <fwData/Vector.hpp>

#include <fwRenderOgre/IAdaptor.hpp>
#include <fwRenderOgre/interactor/IInteractor.hpp>
#include <fwRenderOgre/ITransformable.hpp>
#include <fwRenderOgre/Text.hpp>

class ogreCommand;

namespace visuOgreAdaptor
{

/**
 * @brief Adaptor to display distance on an image
 *
 * @section Slots Slots
 * - \b s_REMOVE_DISTANCE_SLOT: Detection when a point is removed from an image
 *
 * @section XML XML Configuration
 *
 * @code{.xml}
        <service uid="multiDistancesAdp" type="::visuOgreAdaptor::SImageMultiDistances" autoConnect="yes">
            <inout key="image" uid="${image}" />
            <config layer="default" />
        </service>
   @endcode
 * @subsection In-Out In-Out
 * - \b image [::fwData::Image]: image containing the distance field.
 * @subsection Configuration Configuration
 * - \b layer(mandatory) : defines distance's layer.
 */

class SImageMultiDistances : public ::fwRenderOgre::IAdaptor,
                             public ::fwRenderOgre::ITransformable,
                             public ::fwRenderOgre::interactor::IInteractor
{
public:

    fwCoreServiceClassDefinitionsMacro( (SImageMultiDistances)(::fwRenderOgre::IAdaptor) )

    /// Constructor
    VISUOGREADAPTOR_API SImageMultiDistances() noexcept;

    /// Destructor
    VISUOGREADAPTOR_API ~SImageMultiDistances() noexcept;

    /// Behaviour on a MouseMoveEvent
    VISUOGREADAPTOR_API virtual void mouseMoveEvent(MouseButton, int, int, int, int) override;

    /// Behaviour on a WheelEvent
    VISUOGREADAPTOR_API virtual void wheelEvent(int, int, int) override;

    /// Called when the window is resized
    VISUOGREADAPTOR_API virtual void resizeEvent(int, int) override;
    /// Called when a key is pressed
    VISUOGREADAPTOR_API virtual void keyPressEvent(int) override;

    /// Called when a key is release
    VISUOGREADAPTOR_API virtual void keyReleaseEvent(int) override;

    /// Called when a mouse button is released.
    VISUOGREADAPTOR_API virtual void buttonReleaseEvent(MouseButton, int, int) override;

    /// Called when a mouse button is pressed.
    VISUOGREADAPTOR_API virtual void buttonPressEvent(MouseButton, int, int) override;

    /// Called when the focus is win
    VISUOGREADAPTOR_API virtual void focusInEvent() override;

    /// Called when the focus is lost
    VISUOGREADAPTOR_API virtual void focusOutEvent() override;

protected:

    /// Configure the adaptor
    VISUOGREADAPTOR_API void configuring() override;

    /// Initialize sceneManager, rootSceneNode, layer and update the service
    VISUOGREADAPTOR_API void starting() override;

    /// Update distance displaying
    VISUOGREADAPTOR_API void updating() override;

    /// Stop the service
    VISUOGREADAPTOR_API void stopping() override;
    /**
     * @brief Returns proposals to connect service slots to associated object signals,
     * this method is used for obj/srv auto connection
     *
     * Connect Image::s_DISTANCE_ADDED_SIG to this::s_UPDATE_SLOT
     * Connect Image::s_DISTANCE_REMOVED_SIG to this::s_REMOVE_DISTANCE_SLOT
     * Connect Image::s_DISTANCE_DISPLAYED_SIG to this::s_UPDATE_SLOT
     */
    VISUOGREADAPTOR_API virtual KeyConnectionsMap getAutoConnections() const override;

private:

    /// Display line of a given pointlist
    void displayDistance(const ::fwData::PointList::sptr pl);

    /// Remove the distance from a given pointlist and update the service
    void removeDistance(::fwData::PointList::csptr plToRemove);

    /// Remove the distance from a given pointlist
    void removeDistanceVisual();

    /// Create ID label of a given point
    void createIdLabel(float point[3]);

    /// Destroy label corresponding on a specific id
    void destroyLabel(size_t id);

    /// Create millimeter length label of a specific point. The second argument corresponds to the distance
    void createMillimeterLabel(float point[3], const Ogre::Real);

    /// Hide the moving line corresponding to an specific ID
    void hideMovingLine(size_t lineID);

    /// Create a new line. First argument corresponds of the material adaptor. Second argument corresponds to the coord
    /// of the first point
    /// Last argument corresponds to the coord of the second point
    void createLine(const ::visuOgreAdaptor::SMaterial::sptr materialAdp, float ps1[3],
                    float ps2[3]) const;

    /// Check if the mouse click on an existing point. First argument corresponds on the coord of the first point.
    /// Second argument corresponds on the coord of the second point. Last argument corresponds on the coord of the
    /// mouse
    bool clickPoint(float ps1[3], float ps2[3], const ::Ogre::Vector3 worldspaceClikedPoint);

    /// Create a new sphere manual object. First argument corresponds of the material adaptor.
    /// Second argument corresponds on the name of the node
    ::Ogre::ManualObject* createSphere(const ::visuOgreAdaptor::SMaterial::sptr materialAdp,
                                       const std::string name) const;

    /// Set and return the material adaptor
    ::visuOgreAdaptor::SMaterial::sptr setMaterialAdp();

    /// Scene manager of the scene
    ::Ogre::SceneManager* m_sceneMgr { nullptr };

    /// Root manager of the scene
    ::Ogre::SceneNode* m_rootSceneNode { nullptr };

    /// Scene node where point 1 is attached
    ::Ogre::SceneNode* m_point1Node { nullptr };

    /// Scene node where point 2 is attached
    ::Ogre::SceneNode* m_point2Node { nullptr };

    /// Used to store label of each point
    std::vector< ::fwRenderOgre::Text*> m_labels { nullptr };

    /// Used to store the millimeter value for each line
    std::vector< ::fwRenderOgre::Text*> m_millimeterValue { nullptr };

    /// Used to store the id value labels points nodes
    std::vector< ::Ogre::SceneNode*> m_labelNodes { nullptr };

    /// Used to store the length value labels points nodes
    std::vector< ::Ogre::SceneNode*> m_millimeterNodes { nullptr };

    /// Copy of distanceField in case that the signal removed the lign of the image before updating the visual
    ::fwData::Vector::sptr m_distanceField { nullptr };

    /// Number of existing lines
    size_t m_distanceNb { 0 };

    /// Id of the point which will move
    size_t m_moveID  { 0 };

    /// Position (begin/back) of the point which will move
    size_t m_isBeginMove { 0 };

    /// User is moving a point
    bool m_isMovingPoint {false};

    /// Defines whether interaction is possible or not.
    bool m_activeInteraction { false };

    /// Position of the first point
    float m_ps1[3] { 0, 0, 0 };

    /// Position of the second point
    float m_ps2[3] { 0, 0, 0 };

};
}
