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

#include <fwData/PointList.hpp>
#include <fwData/Vector.hpp>

#include <fwRenderOgre/IAdaptor.hpp>
#include <fwRenderOgre/ITransformable.hpp>
#include <fwRenderOgre/interactor/IInteractor.hpp>
#include <fwRenderOgre/Text.hpp>

class ogreCommand;

namespace visuOgreAdaptor
{

/**
 * @brief Adaptor to display distance on an image
 *
 * @section Slots Slots
 * - \b createDistance() : creates a new distance attached to this adaptor
 * - \b removeDistance(::fwData::PointList::sptr) : remove the distance
 *
 * @section XML XML Configuration
 *
 * @code{.xml}

 * @endcode
 * @subsection In-Out In-Out
 * - \b image [::fwData::Image]: image containing the distance field.
 * @subsection Configuration Configuration
 *<config layer="default" />
 **/
class SImageMultiDistances : public ::fwRenderOgre::IAdaptor,
        public ::fwRenderOgre::ITransformable,
        public ::fwRenderOgre::interactor::IInteractor
{
public:

    fwCoreServiceClassDefinitionsMacro( (SImageMultiDistances)(::fwRenderOgre::IAdaptor) )

    VISUOGREADAPTOR_API SImageMultiDistances() noexcept;

    VISUOGREADAPTOR_API ~SImageMultiDistances() noexcept;

    VISUOGREADAPTOR_API virtual void show(bool showDistances = true);

    //VISUOGREADAPTOR_API void setNeedSubservicesDeletion(bool _needSubservicesDeletion);

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
    VISUOGREADAPTOR_API void configuring() override;
    VISUOGREADAPTOR_API void starting() override;
    VISUOGREADAPTOR_API void updating() override;
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

    void displayDistance(::fwData::PointList::sptr pl);

    void removeDistance(::fwData::PointList::csptr plToRemove);

    void createDistance();

    /// Get the distance between two vectors
    Ogre::Real getDistance( Ogre::Vector3 a, Ogre::Vector3 b );

    void removeAllDistance();

    /// Used to create label
    void createLabel(::fwData::Point::csptr);

    /// Used to destroy label
    void destroyLabel(int id);

    void affMillimeter(::fwData::Point::csptr, Ogre::Real);

    ::fwData::Point::cwptr m_point1;

    ::fwData::Point::cwptr m_point2;

    ::Ogre::SceneManager* m_sceneMgr;

    ::Ogre::SceneNode* m_rootSceneNode;

    /// Scene node where point of our manual objects are attached
    ::Ogre::SceneNode* m_pointNode { nullptr };

    /// Scene node where point of our manual objects are attached
    ::Ogre::SceneNode* m_pointNode2 { nullptr };

    /// Used to store label of each point
    std::vector< ::fwRenderOgre::Text*> m_labels;

    /// Used to store millimeter value for each line
    std::vector< ::fwRenderOgre::Text*> m_millimeterValue;

    /// Used to store labels points nodes
    std::vector< ::Ogre::SceneNode*> m_labelNodes;

    /// Used to store labels points nodes
    std::vector< ::Ogre::SceneNode*> m_millimeterNodes;

    /// Copy of distanceField in case that the signal remove the lign from the image before the visual
    ::fwData::Vector::sptr m_distanceField;

    /// Number of visual line
    int _id = 0;

    /// Position of the point I should move
    int _moveID = 0;

    /// Begin or End point from the poinlist
    int _isBeginMove {0};

    /// Is the user currently displacing a point.
    bool m_isMovingPoint {false};

    /// Defines whether interaction is possible or not.
    bool m_activeInteraction { false };

    bool checkMove(double ps1[3], double ps2[3], const ::Ogre::Vector3 worldspaceClikedPoint);

    bool matchPointToRemove(int toFind, int searchBigger);

    double _ps1[3];

    double _ps2[3];

    /// Hide one line, the lineID is the position of the line that you wanna hide
    void hideLine(int lineID);
};
}
