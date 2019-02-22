/************************************************************************
 *
 * Copyright (C) 2018-2019 IRCAD France
 * Copyright (C) 2018-2019 IHU Strasbourg
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

#include "visuOgreAdaptor/SImageMultiDistances.hpp"

#include <fwCom/Signal.hpp>
#include <fwCom/Signal.hxx>
#include <fwCom/Slot.hpp>
#include <fwCom/Slot.hxx>
#include <fwCom/Slots.hpp>
#include <fwCom/Slots.hxx>

#include <fwData/Boolean.hpp>
#include <fwData/Image.hpp>
#include <fwData/mt/ObjectReadLock.hpp>
#include <fwData/mt/ObjectReadToWriteLock.hpp>
#include <fwData/mt/ObjectWriteLock.hpp>
#include <fwData/PointList.hpp>
#include <fwData/String.hpp>
#include <fwData/Vector.hpp>

#include <fwDataTools/fieldHelper/Image.hpp>
#include <fwDataTools/helper/Array.hpp>
#include <fwDataTools/helper/Mesh.hpp>
#include <fwDataTools/helper/Vector.hpp>

#include <fwRenderOgre/helper/Camera.hpp>
#include <fwRenderOgre/helper/Font.hpp>
#include <fwRenderOgre/helper/ManualObject.hpp>
#include <fwRenderOgre/helper/Scene.hpp>
#include <fwRenderOgre/R2VBRenderable.hpp>
#include <fwRenderOgre/SRender.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/op/Add.hpp>

#include <OgreVector3.h>

namespace visuOgreAdaptor
{

fwServicesRegisterMacro( ::fwRenderOgre::IAdaptor, ::visuOgreAdaptor::SImageMultiDistances);

static const ::fwServices::IService::KeyType s_IMAGE_INOUT      = "image";
static const ::fwServices::IService::KeyType s_POINTLIST_INPUT  = "pointList";
static const ::fwCom::Slots::SlotKeyType s_REMOVE_DISTANCE_SLOT = "removeDistance";

SImageMultiDistances::SImageMultiDistances() noexcept
{
    newSlot(s_REMOVE_DISTANCE_SLOT, &SImageMultiDistances::removeDistance, this);
}

//------------------------------------------------------------------------------

SImageMultiDistances::~SImageMultiDistances() noexcept
{

}

//------------------------------------------------------------------------------

void SImageMultiDistances::configuring()
{
    this->configureParams();
}

//------------------------------------------------------------------------------

void SImageMultiDistances::starting()
{
    this->initialize();
    this->getRenderService()->makeCurrent();
    m_sceneMgr      = this->getSceneManager();
    m_rootSceneNode = this->getSceneManager()->getRootSceneNode();
    this->updating();
    auto layer = this->getRenderService()->getLayer(m_layerID);
    layer->setSelectInteractor(std::dynamic_pointer_cast< ::fwRenderOgre::interactor::IInteractor >(this->getSptr()));
}

//------------------------------------------------------------------------------

void SImageMultiDistances::updating()
{
    ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);
    SLM_ASSERT("Missing image", image);
    ::fwData::Vector::sptr distanceField;
    distanceField = image->getField< ::fwData::Vector >( ::fwDataTools::fieldHelper::Image::m_imageDistancesId);
    this->removeDistanceVisual();

    bool isShown = image->getField("ShowDistances", ::fwData::Boolean::New(true))->value();
    if( distanceField && isShown == true )
    {
        m_distanceField = distanceField;
        for(::fwData::Object::sptr object :  *m_distanceField)
        {
            ::fwData::PointList::sptr distance = ::fwData::PointList::dynamicCast(object);
            SLM_ASSERT( "Empty Point List for Distance !!!!", !distance->getPoints().empty() );
            this->displayDistance(distance);
            ++m_distanceNb;
        }
    }
    this->requestRender();
}

//------------------------------------------------------------------------------

void SImageMultiDistances::createMillimeterLabel(float point[3], const Ogre::Real distance)
{
    ::Ogre::OverlayContainer* textContainer = this->getRenderService()->getOverlayTextPanel();
    ::Ogre::FontPtr dejaVuSansFont          = ::fwRenderOgre::helper::Font::getFont("DejaVuSans.ttf", 32);
    ::Ogre::Camera* cam                     = this->getLayer()->getDefaultCamera();
    std::string labelNumber = std::to_string(distance) + "mm";

    m_millimeterValue.push_back(::fwRenderOgre::Text::New(this->getID() + labelNumber + std::to_string(m_distanceNb),
                                                          m_sceneMgr,
                                                          textContainer,
                                                          dejaVuSansFont, cam));
    m_millimeterValue[m_distanceNb]->setText(labelNumber);
    m_millimeterValue[m_distanceNb]->setCharHeight(0.03f);
    m_millimeterValue[m_distanceNb]->setTextColor(::Ogre::ColourValue(0, 0, 255));

    m_millimeterNodes.push_back(m_rootSceneNode->createChildSceneNode(this->getID() + "_distance" +
                                                                      std::to_string(m_distanceNb)));
    m_millimeterNodes[m_distanceNb]->attachObject(m_millimeterValue[m_distanceNb]);

    m_millimeterNodes[m_distanceNb]->translate(point[0], point[1], 0.01f);
}

//------------------------------------------------------------------------------

void SImageMultiDistances::createIdLabel(float ps1[3])
{
    ::Ogre::OverlayContainer* textContainer = this->getRenderService()->getOverlayTextPanel();
    ::Ogre::FontPtr dejaVuSansFont          = ::fwRenderOgre::helper::Font::getFont("DejaVuSans.ttf", 32);
    ::Ogre::Camera* cam                     = this->getLayer()->getDefaultCamera();
    std::string labelNumber = std::to_string(m_distanceNb);

    m_labels.push_back(::fwRenderOgre::Text::New(this->getID() + labelNumber, m_sceneMgr, textContainer,
                                                 dejaVuSansFont, cam));
    m_labels[m_distanceNb]->setText(labelNumber);
    m_labels[m_distanceNb]->setCharHeight(0.03f);
    m_labels[m_distanceNb]->setTextColor(::Ogre::ColourValue(0, 0, 255));

    m_labelNodes.push_back(m_rootSceneNode->createChildSceneNode(this->getID() + "_id" + labelNumber));
    m_labelNodes[m_distanceNb]->attachObject(m_labels[m_distanceNb]);
    m_labelNodes[m_distanceNb]->translate(ps1[0], ps1[1], 0.01f);
}

//------------------------------------------------------------------------------

void SImageMultiDistances::createLine(const ::visuOgreAdaptor::SMaterial::sptr materialAdp,
                                      float ps1[3], float ps2[3]) const
{
    ::Ogre::ManualObject* line =
        m_sceneMgr->createManualObject(this->getID() + "_line" + std::to_string(m_distanceNb));

    /// Draw the line
    line->begin(materialAdp->getMaterialName(), ::Ogre::RenderOperation::OT_LINE_LIST);
    line->position(ps1[0], ps1[1], 0.01f);
    line->colour(::Ogre::ColourValue::Blue);
    line->position(ps2[0], ps2[1], 0.01f);
    line->colour(::Ogre::ColourValue::Blue);
    line->end();
    m_rootSceneNode->attachObject(line);
}

//------------------------------------------------------------------------------

::Ogre::ManualObject* SImageMultiDistances::createSphere(const ::visuOgreAdaptor::SMaterial::sptr materialAdp,
                                                         const std::string name) const
{
    ::Ogre::ManualObject* sphere =
        m_sceneMgr->createManualObject(this->getID() + name + std::to_string(m_distanceNb));
    ::fwRenderOgre::helper::ManualObject::createSphere(sphere,
                                                       materialAdp->getMaterialName(), ::Ogre::ColourValue::Blue,
                                                       1.f);
    return sphere;
}

//------------------------------------------------------------------------------

::visuOgreAdaptor::SMaterial::sptr SImageMultiDistances::setMaterialAdp()
{
    ::fwData::Material::sptr material = ::fwData::Material::New();

    ::visuOgreAdaptor::SMaterial::sptr materialAdp = this->registerService< ::visuOgreAdaptor::SMaterial >(
        "::visuOgreAdaptor::SMaterial");
    materialAdp->registerInOut(material, ::visuOgreAdaptor::SMaterial::s_MATERIAL_INOUT, true);
    materialAdp->setID(this->getID() + materialAdp->getID());
    materialAdp->setMaterialName(this->getID() + materialAdp->getID());
    materialAdp->setRenderService( this->getRenderService() );
    materialAdp->setLayerID(m_layerID);
    materialAdp->setShadingMode("ambient");
    materialAdp->setMaterialTemplateName(::fwRenderOgre::Material::DEFAULT_MATERIAL_TEMPLATE_NAME);
    materialAdp->start();

    materialAdp->getMaterialFw()->setHasVertexColor(true);
    materialAdp->update();
    return materialAdp;
}

//------------------------------------------------------------------------------

void SImageMultiDistances::displayDistance(const ::fwData::PointList::sptr pl)
{
    if ( pl->getPoints().size() > 1 )
    {
        ::fwData::Point::cwptr pointFront = pl->getPoints().front();
        ::fwData::Point::cwptr pointBack  = pl->getPoints().back();
        ::fwData::Point::csptr p1         = pointFront.lock();
        ::fwData::Point::csptr p2         = pointBack.lock();

        ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);

        float ps1[3];
        float ps2[3];
        std::copy(p1->getCoord().begin(), (p1)->getCoord().end(), ps1 );
        std::copy(p2->getCoord().begin(), (p2)->getCoord().end(), ps2 );

        /// Convert value for taking into account the origin and the spacing of the image
        ps1[0] =
            static_cast<float>((static_cast<double>(ps1[0]) - image->getOrigin()[0] ) / image->getSpacing()[0] +0.01);
        ps1[1] =
            static_cast<float>((static_cast<double>(ps1[1]) - image->getOrigin()[1] ) / image->getSpacing()[1] +0.01);
        ps2[0] =
            static_cast<float>((static_cast<double>(ps2[0]) - image->getOrigin()[0] ) / image->getSpacing()[0] +0.01);
        ps2[1] =
            static_cast<float>((static_cast<double>(ps2[1]) - image->getOrigin()[1] ) / image->getSpacing()[1] +0.01);

        ::visuOgreAdaptor::SMaterial::sptr materialAdp = setMaterialAdp();
        this->createLine(materialAdp, ps1, ps2);
        ::Ogre::ManualObject* sphere  = createSphere(materialAdp, "_sphere1");
        ::Ogre::ManualObject* sphere2 = createSphere(materialAdp, "_sphere2");

        /// Create the ID of the line and aff the value of the line (in millimeter)
        this->createIdLabel(ps1);
        Ogre::Vector3 a = Ogre::Vector3(static_cast<Ogre::Real>(ps1[0]), static_cast<Ogre::Real>(ps1[1]), 0);
        Ogre::Vector3 b = Ogre::Vector3(static_cast<Ogre::Real>(ps2[0]), static_cast<Ogre::Real>(ps2[1]), 0);
        this->createMillimeterLabel(ps2, a.distance(b));

        /// Attach the object on the different node
        m_point1Node =
            this->m_rootSceneNode->createChildSceneNode(this->getID() + "_point" + "1" + std::to_string(m_distanceNb));
        m_point2Node =
            this->m_rootSceneNode->createChildSceneNode(this->getID() + "_point" + "2" + std::to_string(m_distanceNb));
        m_point1Node->attachObject(sphere);
        m_point1Node->translate(ps1[0], ps1[1], 0);
        m_point2Node->attachObject(sphere2);
        m_point2Node->translate(ps2[0], ps2[1], 0);
    }
}

//------------------------------------------------------------------------------

void SImageMultiDistances::stopping()
{
}

//------------------------------------------------------------------------------

void SImageMultiDistances::destroyLabel(size_t id)
{
    m_rootSceneNode->removeAndDestroyChild(this->getID() + "_id" + std::to_string(id));
    m_rootSceneNode->removeAndDestroyChild(this->getID() + "_distance" + std::to_string(id));

    m_rootSceneNode->removeAndDestroyChild(this->getID() + "_point" + "1" + std::to_string(id));
    m_rootSceneNode->removeAndDestroyChild(this->getID() + "_point" + "2" + std::to_string(id));

    m_sceneMgr->destroyMovableObject(m_labels[id]);
    m_sceneMgr->destroyMovableObject(m_millimeterValue[id]);
}

//------------------------------------------------------------------------------

void SImageMultiDistances::hideMovingLine(size_t lineID)
{
    ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);
    SLM_ASSERT("Missing image", image);

    for (size_t id = 0; id < m_distanceNb; ++id)
    {
        if (m_sceneMgr->hasManualObject(this->getID() + "_line" + std::to_string(id)) && lineID == id)
        {
            ::Ogre::ManualObject* line =
                m_sceneMgr->getManualObject(this->getID() + "_line" + std::to_string(id));
            ::Ogre::ManualObject* sphere = m_sceneMgr->getManualObject(
                this->getID() + "_sphere" + "1" + std::to_string(id));
            ::Ogre::ManualObject* sphere2 = m_sceneMgr->getManualObject(
                this->getID() + "_sphere" + "2" +std::to_string(id));
            this->destroyLabel(id);
            m_sceneMgr->destroyManualObject(sphere);
            m_sceneMgr->destroyManualObject(sphere2);
            m_sceneMgr->destroyManualObject(line);
        }
    }
}

//------------------------------------------------------------------------------

void SImageMultiDistances::removeDistanceVisual()
{
    if( m_distanceField )
    {
        for (size_t id = 0; id < m_distanceNb; ++id)
        {
            if (m_sceneMgr->hasManualObject(this->getID() + "_line" + std::to_string(id)))
            {
                ::Ogre::ManualObject* line =
                    m_sceneMgr->getManualObject(this->getID() + "_line" + std::to_string(id));
                ::Ogre::ManualObject* sphere = m_sceneMgr->getManualObject(
                    this->getID() + "_sphere" + "1" + std::to_string(id));
                ::Ogre::ManualObject* sphere2 = m_sceneMgr->getManualObject(
                    this->getID() + "_sphere" + "2" +std::to_string(id));
                this->destroyLabel(id);
                m_sceneMgr->destroyManualObject(sphere);
                m_sceneMgr->destroyManualObject(sphere2);
                m_sceneMgr->destroyManualObject(line);
            }
        }
    }
    m_distanceNb = 0;
    m_labels.clear();
    m_millimeterValue.clear();
    m_labelNodes.clear();
    m_millimeterNodes.clear();
}

//------------------------------------------------------------------------------

void SImageMultiDistances::removeDistance(::fwData::PointList::csptr)
{
    this->removeDistanceVisual();
    this->updating();
}

//------------------------------------------------------------------------------

void SImageMultiDistances::mouseMoveEvent(MouseButton button, int x, int y, int, int)
{
    if (button != MouseButton::LEFT || !m_activeInteraction)
    {
        return;
    }
    if (m_sceneMgr->hasManualObject(this->getID() + "_movingLine"))
    {
        ::Ogre::ManualObject* line = m_sceneMgr->getManualObject(this->getID() + "_movingLine");
        m_sceneMgr->destroyManualObject(line);
        this->hideMovingLine(m_moveID);
    }
    ::visuOgreAdaptor::SMaterial::sptr materialAdp = this->setMaterialAdp();

    /// Set the current cursor position to ogre position
    ::fwData::Mesh::PointValueType clickedPoint[3] {static_cast<float>(x), static_cast<float>(y), 0};
    const ::Ogre::Vector3 worldspaceClikedPoint = ::fwRenderOgre::helper::Camera::convertPixelToViewSpace(
        *(this->getLayer()->getDefaultCamera()), clickedPoint);

    ::Ogre::ManualObject* line = m_sceneMgr->createManualObject(this->getID() + "_movingLine");
    line->begin(materialAdp->getMaterialName(), ::Ogre::RenderOperation::OT_LINE_LIST);
    if (m_isBeginMove == 1)
    {
        line->position(m_ps1[0], m_ps1[1], 1.0f);
        line->colour(::Ogre::ColourValue::Red);
        line->position(worldspaceClikedPoint.x, worldspaceClikedPoint.y, 1.0f);
    }
    else
    {
        line->position(worldspaceClikedPoint.x, worldspaceClikedPoint.y, 1.0f);
        line->colour(::Ogre::ColourValue::Red);
        line->position(m_ps2[0], m_ps2[1], 1.0f);
    }
    line->end();
    m_rootSceneNode->attachObject(line);
}

//------------------------------------------------------------------------------

void SImageMultiDistances::wheelEvent(int, int, int)
{
}

//------------------------------------------------------------------------------

void SImageMultiDistances::resizeEvent(int, int)
{
}

//------------------------------------------------------------------------------

void SImageMultiDistances::keyPressEvent(int)
{
}

//------------------------------------------------------------------------------

void SImageMultiDistances::keyReleaseEvent(int)
{
}

//------------------------------------------------------------------------------

void SImageMultiDistances::buttonReleaseEvent(MouseButton button, int x, int y)
{
    if (button != MouseButton::LEFT || !m_activeInteraction)
    {
        return;
    }
    if (m_sceneMgr->hasManualObject(this->getID() + "_movingLine"))
    {
        ::Ogre::ManualObject* line = m_sceneMgr->getManualObject(this->getID() + "_movingLine");
        m_sceneMgr->destroyManualObject(line);
    }

    ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);
    /// Set the current cursor position to ogre position
    ::fwData::Mesh::PointValueType clickedPoint[3] {static_cast<float>(x), static_cast<float>(y), 0};
    ::Ogre::Vector3 worldspaceClikedPoint = ::fwRenderOgre::helper::Camera::convertPixelToViewSpace(
        *(this->getLayer()->getDefaultCamera()), clickedPoint);

    ::fwData::Vector::sptr distanceField = image->getField< ::fwData::Vector >(
        ::fwDataTools::fieldHelper::Image::m_imageDistancesId);
    ::fwData::Vector::ContainerType& container = distanceField->getContainer();

    size_t id = 0;
    /// Convert value for NOT taking into account the origin and the spacing of the image
    worldspaceClikedPoint.x =
        static_cast<float>((static_cast<double>(worldspaceClikedPoint.x) * image->getSpacing()[0]) +
                           image->getOrigin()[0]);
    worldspaceClikedPoint.y =
        static_cast<float>((static_cast<double>(worldspaceClikedPoint.y) * image->getSpacing()[1]) +
                           image->getOrigin()[1]);

    for(auto it = container.begin(); it != container.end(); ++it)
    {
        if (id == m_moveID)
        {
            ::fwData::PointList::sptr pointList = ::fwData::PointList::dynamicCast(*it);
            pointList->getPoints()[m_isBeginMove]->setCoord({static_cast<double>(
                                                                 worldspaceClikedPoint.x),
                                                             static_cast<double>(
                                                                 worldspaceClikedPoint.y)});
        }
        id++;
    }
    auto sigImage = image->signal< ::fwData::Image::ModifiedSignalType >(::fwData::Image::s_MODIFIED_SIG);
    sigImage->asyncEmit();
    this->updating();
}

//------------------------------------------------------------------------------

bool SImageMultiDistances::clickPoint(double ps1[3], double ps2[3],
                                      const ::Ogre::Vector3 worldspaceClikedPoint)
{
    if (std::abs(static_cast<float>(ps1[0]) - worldspaceClikedPoint.x) <= 2 &&
        std::abs(static_cast<float>(ps1[1]) - worldspaceClikedPoint.y) <= 2)
    {
        m_isBeginMove = 0;
        return true;
    }
    else if (std::abs(static_cast<float>(ps2[0]) - worldspaceClikedPoint.x) <= 2 &&
             std::abs(static_cast<float>(ps2[1]) - worldspaceClikedPoint.y) <= 2)
    {
        m_isBeginMove = 1;
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------

void SImageMultiDistances::buttonPressEvent(MouseButton button, int x, int y)
{
    if (button != MouseButton::LEFT)
    {
        return;
    }
    m_moveID            = 0;
    m_activeInteraction = false;

    ::fwData::Mesh::PointValueType clickedPoint[3] {static_cast<float>(x), static_cast<float>(y), 0};
    const ::Ogre::Vector3 worldspaceClikedPoint = ::fwRenderOgre::helper::Camera::convertPixelToViewSpace(
        *(this->getLayer()->getDefaultCamera()), clickedPoint);

    ::fwData::Image::sptr image          = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);
    ::fwData::Vector::sptr distanceField = image->getField< ::fwData::Vector >(
        ::fwDataTools::fieldHelper::Image::m_imageDistancesId);

    size_t id = 0;
    for(::fwData::Object::sptr object :  *distanceField)
    {
        ::fwData::PointList::sptr distance = ::fwData::PointList::dynamicCast(object);
        SLM_ASSERT( "Empty Point List for Distance !!!!", !distance->getPoints().empty() );
        ::fwData::Point::cwptr pointFront = distance->getPoints().front();
        ::fwData::Point::cwptr pointBack  = distance->getPoints().back();
        ::fwData::Point::csptr p1         = pointFront.lock();
        ::fwData::Point::csptr p2         = pointBack.lock();

        double ps1[3];
        double ps2[3];
        std::copy(p1->getCoord().begin(), (p1)->getCoord().end(), ps1 );
        std::copy(p2->getCoord().begin(), (p2)->getCoord().end(), ps2 );

        /// Convert value for taking into account the origin and the spacing of the image
        ps1[0] =
            (ps1[0] - image->getOrigin()[0] ) / image->getSpacing()[0] +0.01;
        ps1[1] =
            (ps1[1] - image->getOrigin()[1] ) / image->getSpacing()[1] +0.01;
        ps2[0] =
            (ps2[0] - image->getOrigin()[0] ) / image->getSpacing()[0] +0.01;
        ps2[1] =
            (ps2[1] - image->getOrigin()[1] ) / image->getSpacing()[1] +0.01;

        if (this->clickPoint(ps1, ps2, worldspaceClikedPoint) == true)
        {
            m_moveID = id;
            std::copy(std::begin(ps1), std::end(ps1), m_ps1 );
            std::copy(std::begin(ps2), std::end(ps2), m_ps2 );
            m_activeInteraction = true;
        }
        ++id;
    }
}

//------------------------------------------------------------------------------

void SImageMultiDistances::focusInEvent()
{
}

//------------------------------------------------------------------------------

void SImageMultiDistances::focusOutEvent()
{
}

//------------------------------------------------------------------------------

::fwServices::IService::KeyConnectionsMap SImageMultiDistances::getAutoConnections() const
{
    KeyConnectionsMap connections;

    connections.push(s_IMAGE_INOUT, ::fwData::Image::s_DISTANCE_ADDED_SIG, s_UPDATE_SLOT);
    connections.push(s_IMAGE_INOUT, ::fwData::Image::s_DISTANCE_REMOVED_SIG, s_REMOVE_DISTANCE_SLOT);
    connections.push(s_IMAGE_INOUT, ::fwData::Image::s_DISTANCE_DISPLAYED_SIG, s_UPDATE_SLOT);
    return connections;
}

}
