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

#include "visuOgreAdaptor/SImageMultiDistances.hpp"

#include "visuOgreAdaptor/SLine.hpp"

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
#include <fwDataTools/helper/Vector.hpp>

#include <fwDataTools/helper/Array.hpp>
#include <fwRenderOgre/helper/Camera.hpp>
#include <fwRenderOgre/helper/Font.hpp>
#include <fwRenderOgre/helper/ManualObject.hpp>
#include <fwDataTools/helper/Mesh.hpp>
#include <fwRenderOgre/helper/Scene.hpp>
#include <fwRenderOgre/R2VBRenderable.hpp>
#include <fwRenderOgre/SRender.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/op/Add.hpp>

#include <OgreVector3.h>

fwServicesRegisterMacro( ::fwRenderOgre::IAdaptor, ::visuOgreAdaptor::SImageMultiDistances);

namespace visuOgreAdaptor
{

const ::fwServices::IService::KeyType s_IMAGE_INOUT             = "image";
const ::fwServices::IService::KeyType s_POINTLIST_INPUT         = "pointList";
static const ::fwCom::Slots::SlotKeyType s_REMOVE_DISTANCE_SLOT = "removeDistance";

SImageMultiDistances::SImageMultiDistances() noexcept
{
    newSlot(s_REMOVE_DISTANCE_SLOT, &SImageMultiDistances::removeDistance, this);
}

SImageMultiDistances::~SImageMultiDistances() noexcept
{

}

//------------------------------------------------------------------------------

void SImageMultiDistances::configuring()
{
    this->configureParams();
}

//------------------------------------------------------------------------------

::fwData::Color::sptr generateColor()
{
    using namespace fwData;

    static std::vector< Color::sptr > colors;
    static std::vector< Color::sptr >::iterator current;
    if ( colors.empty() )
    {
        Color::sptr magenta = Color::New();
        magenta->setRGBA( 1, 0, 1);
        colors.push_back( magenta );

        Color::sptr cyan = Color::New();
        cyan->setRGBA(0, 1, 1);
        colors.push_back( cyan );

        Color::sptr orange = Color::New();
        orange->setRGBA(  1, 0.647f, 0);
        colors.push_back( orange );

        Color::sptr violet = Color::New();
        violet->setRGBA( .5f, 0.26f, 1);
        colors.push_back( violet );

        Color::sptr vertpomme = Color::New();
        vertpomme->setRGBA( .65f, 1, 0);
        colors.push_back( vertpomme );

        Color::sptr jaune = Color::New();
        jaune->setRGBA( 1, 1, 0);
        colors.push_back( jaune );

        current = colors.begin();
    }

    if ( ++current == colors.end() )
    {
        current = colors.begin();
    }

    Color::sptr newColor;
    newColor = ::fwData::Object::copy( *current );
    return newColor;
}

//------------------------------------------------------------------------------

void SImageMultiDistances::starting()
{
    this->initialize();
    this->getRenderService()->makeCurrent();
    auto layer= this->getRenderService()->getLayer(m_layerID);
    layer->setSelectInteractor(std::dynamic_pointer_cast< ::fwRenderOgre::interactor::IInteractor >(this->getSptr()));
    m_sceneMgr      = this->getSceneManager();
    m_rootSceneNode = this->getSceneManager()->getRootSceneNode();
    this->updating();
}

//------------------------------------------------------------------------------

void SImageMultiDistances::updating()
{
    // get PointList in image Field then install distance service if required
    this->requestRender();
    ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);
    SLM_ASSERT("Missing image", image);

    ::fwData::Vector::sptr distanceField;
    distanceField = image->getField< ::fwData::Vector >( ::fwDataTools::fieldHelper::Image::m_imageDistancesId);
    bool isShown = image->getField("ShowDistances", ::fwData::Boolean::New(true))->value();
    removeAllDistance();
    if( distanceField && isShown == true )
    {
        m_distanceField = distanceField;
        for(::fwData::Object::sptr object :  *distanceField)
        {
            ::fwData::PointList::sptr distance    = ::fwData::PointList::dynamicCast(object);
            SLM_ASSERT( "Empty Point List for Distance !!!!", !distance->getPoints().empty() );
            this->displayDistance(distance);
            _id++;
        }
    }
}

//------------------------------------------------------------------------------

Ogre::Real SImageMultiDistances::getDistance( Ogre::Vector3 a, Ogre::Vector3 b )
{
    return ((a - b).length());
}

void SImageMultiDistances::affMillimeter(::fwData::Point::csptr point, Ogre::Real distance)
{
    ::Ogre::OverlayContainer* textContainer = this->getRenderService()->getOverlayTextPanel();
    ::Ogre::FontPtr dejaVuSansFont          = ::fwRenderOgre::helper::Font::getFont("DejaVuSans.ttf", 32);
    ::Ogre::Camera* cam                     = this->getLayer()->getDefaultCamera();
    SLM_ASSERT("::Ogre::SceneManager is null", m_sceneMgr);
    std::string labelNumber = std::to_string(distance) + "mm";

    m_millimeterValue.push_back(::fwRenderOgre::Text::New(this->getID() + labelNumber + std::to_string(_id), m_sceneMgr,
                                                          textContainer,
                                                          dejaVuSansFont, cam));
    m_millimeterValue[static_cast<size_t>(_id)]->setText(labelNumber);
    m_millimeterValue[static_cast<size_t>(_id)]->setCharHeight(static_cast<float>(0.03));
    m_millimeterValue[static_cast<size_t>(_id)]->setTextColor(::Ogre::ColourValue(0, 0, 255));
    m_millimeterNodes.push_back(m_rootSceneNode->createChildSceneNode(this->getID() + "_distance" +
                                                                      std::to_string(_id)));
    m_millimeterNodes[static_cast<size_t>(_id)]->attachObject(m_millimeterValue[static_cast<size_t>(_id)]);
    ::fwData::Point::PointCoordArrayType coord = point->getCoord();
    m_millimeterNodes[static_cast<size_t>(_id)]->translate(static_cast<float>(coord[0]),
            static_cast<float>(coord[1]),
            static_cast<float>(0.01));
}

//------------------------------------------------------------------------------

void SImageMultiDistances::createLabel(::fwData::Point::csptr point)
{
    ::Ogre::OverlayContainer* textContainer = this->getRenderService()->getOverlayTextPanel();
    ::Ogre::FontPtr dejaVuSansFont          = ::fwRenderOgre::helper::Font::getFont("DejaVuSans.ttf", 32);
    ::Ogre::Camera* cam                     = this->getLayer()->getDefaultCamera();
    SLM_ASSERT("::Ogre::SceneManager is null", m_sceneMgr);
    std::string labelNumber = std::to_string(_id);

    m_labels.push_back(::fwRenderOgre::Text::New(this->getID() + labelNumber, m_sceneMgr, textContainer,
                                                 dejaVuSansFont, cam));
    m_labels[static_cast<size_t>(_id)]->setText(labelNumber);
    m_labels[static_cast<size_t>(_id)]->setCharHeight(static_cast<float>(0.03));
    m_labels[static_cast<size_t>(_id)]->setTextColor(::Ogre::ColourValue(0, 0, 255));
    m_labelNodes.push_back(m_rootSceneNode->createChildSceneNode(this->getID() + "_id" + labelNumber));
    m_labelNodes[static_cast<size_t>(_id)]->attachObject(m_labels[static_cast<size_t>(_id)]);
    ::fwData::Point::PointCoordArrayType coord = point->getCoord();
    m_labelNodes[static_cast<size_t>(_id)]->translate(static_cast<float>(coord[0]),
            static_cast<float>(coord[1]),
            static_cast<float>(0.01));
}

//------------------------------------------------------------------------------

void SImageMultiDistances::displayDistance( ::fwData::PointList::sptr pl)
{
    if ( pl->getPoints().size() > 1 )
    {
        m_point1 = pl->getPoints().front();
        m_point2 = pl->getPoints().back();
        pl->setDefaultField( ::fwDataTools::fieldHelper::Image::m_colorId, generateColor() );
        ::fwData::Point::csptr p1 = m_point1.lock();
        ::fwData::Point::csptr p2 = m_point2.lock();

        double ps1[3];
        std::copy(p1->getCoord().begin(), (p1)->getCoord().end(), ps1 );

        double ps2[3];
        std::copy(p2->getCoord().begin(), (p2)->getCoord().end(), ps2 );

        ::Ogre::ManualObject* m_line = m_sceneMgr->createManualObject(this->getID() + "_line" + std::to_string(_id));
        /// Set the material
        ::fwData::Material::sptr m_material = ::fwData::Material::New();

        ::Ogre::ColourValue m_color(static_cast<float>(0) / 255.f, static_cast<float>(0) / 255.f, static_cast<float>(255) / 255.f, static_cast<float>(255) / 255.f);

        ::visuOgreAdaptor::SMaterial::sptr materialAdaptor = this->registerService< ::visuOgreAdaptor::SMaterial >(
                    "::visuOgreAdaptor::SMaterial");
        materialAdaptor->registerInOut(m_material, ::visuOgreAdaptor::SMaterial::s_MATERIAL_INOUT, true);
        materialAdaptor->setID(this->getID() + materialAdaptor->getID());
        materialAdaptor->setMaterialName(this->getID() + materialAdaptor->getID());
        materialAdaptor->setRenderService( this->getRenderService() );
        materialAdaptor->setLayerID(m_layerID);
        materialAdaptor->setShadingMode("ambient");
        materialAdaptor->setMaterialTemplateName(::fwRenderOgre::Material::DEFAULT_MATERIAL_TEMPLATE_NAME);
        materialAdaptor->start();

        materialAdaptor->getMaterialFw()->setHasVertexColor(true);
        materialAdaptor->update();

        /// Draw the line
        m_line->begin(materialAdaptor->getMaterialName(), ::Ogre::RenderOperation::OT_LINE_LIST);
        m_line->position(static_cast<float>(ps1[0]), static_cast<float>(ps1[1]),
                static_cast<float>(0.01));
        m_line->colour(m_color);
        m_line->position(static_cast<float>(ps2[0]), static_cast<float>(ps2[1]),
                static_cast<float>(0.01));
        m_line->colour(m_color);
        m_line->end();
        m_rootSceneNode->attachObject(m_line);


        /// Create the sphere on the ends of each line
        ::Ogre::ManualObject* sphere =
                m_sceneMgr->createManualObject(this->getID() + "_sphere" + "1" + std::to_string(_id));
        ::fwRenderOgre::helper::ManualObject::createSphere(sphere,
                                                           materialAdaptor->getMaterialName(), m_color,
                                                           static_cast<float>(1));

        ::Ogre::ManualObject* sphere2 =
                m_sceneMgr->createManualObject(this->getID() + "_sphere" + "2" + std::to_string(_id));
        ::fwRenderOgre::helper::ManualObject::createSphere(sphere2,
                                                           materialAdaptor->getMaterialName(), m_color,
                                                           static_cast<float>(1));

        /// Create the ID of the line and aff the value of the line (in millimeter)
        createLabel(p1);
        Ogre::Vector3 a = Ogre::Vector3(static_cast<Ogre::Real>(ps1[0]), static_cast<Ogre::Real>(ps1[1]), 0);
        Ogre::Vector3 b = Ogre::Vector3(static_cast<Ogre::Real>(ps2[0]), static_cast<Ogre::Real>(ps2[1]), 0);
        affMillimeter(p2, getDistance(a, b));

        /// Attach the object on the different node
        m_pointNode =
                this->m_rootSceneNode->createChildSceneNode(this->getID() + "_point" + "1" + std::to_string(_id));
        m_pointNode2 =
                this->m_rootSceneNode->createChildSceneNode(this->getID() + "_point" + "2" + std::to_string(_id));
        m_pointNode->attachObject(sphere);
        m_pointNode->translate(static_cast<float>(ps1[0]), static_cast<float>(ps1[1]), 0);
        m_pointNode2->attachObject(sphere2);
        m_pointNode2->translate(static_cast<float>(ps2[0]), static_cast<float>(ps2[1]), 0);
    }
}

//------------------------------------------------------------------------------

void SImageMultiDistances::stopping()
{
}

//------------------------------------------------------------------------------

void SImageMultiDistances::show(bool showDistances)
{
    if (showDistances)
    {
        this->starting();
    }
    else
    {
        this->stopping();
    }
}

//------------------------------------------------------------------------------

void SImageMultiDistances::destroyLabel(int id)
{
    m_rootSceneNode->removeAndDestroyChild(this->getID() + "_id" + std::to_string(id));
    m_rootSceneNode->removeAndDestroyChild(this->getID() + "_distance" + std::to_string(id));

    m_rootSceneNode->removeAndDestroyChild(this->getID() + "_point" + "1" + std::to_string(id));
    m_rootSceneNode->removeAndDestroyChild(this->getID() + "_point" + "2" + std::to_string(id));

    m_sceneMgr->destroyMovableObject(m_labels[static_cast<size_t>(id)]);
    m_sceneMgr->destroyMovableObject(m_millimeterValue[static_cast<size_t>(id)]);
}

//------------------------------------------------------------------------------

void SImageMultiDistances::hideLine(int lineID)
{
    ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);
    SLM_ASSERT("Missing image", image);
    ::fwData::Vector::sptr distanceField = image->getField< ::fwData::Vector >( ::fwDataTools::fieldHelper::Image::m_imageDistancesId);

    if (!distanceField)
    {
        distanceField = m_distanceField;
    }
    if( distanceField )
    {
        for (int i = 0; i < _id; ++i)
        {
            if (m_sceneMgr->hasManualObject(this->getID() + "_line" + std::to_string(i)) && lineID == i)
            {
                ::Ogre::ManualObject* line =
                        m_sceneMgr->getManualObject(this->getID() + "_line" + std::to_string(i));
                ::Ogre::ManualObject* sphere = m_sceneMgr->getManualObject(
                            this->getID() + "_sphere" + "1" + std::to_string(i));
                ::Ogre::ManualObject* sphere2 = m_sceneMgr->getManualObject(
                            this->getID() + "_sphere" + "2" +std::to_string(i));
                destroyLabel(i);
                m_sceneMgr->destroyManualObject(sphere);
                m_sceneMgr->destroyManualObject(sphere2);
                m_sceneMgr->destroyManualObject(line);
            }
        }
    }
}

void SImageMultiDistances::removeAllDistance()
{
    ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);
    SLM_ASSERT("Missing image", image);
    ::fwData::Vector::sptr distanceField =image->getField< ::fwData::Vector >( ::fwDataTools::fieldHelper::Image::m_imageDistancesId);

    if (!distanceField)
    {
        distanceField = m_distanceField;
    }
    if( distanceField )
    {
        for (int i = 0; i < _id; ++i)
        {
            if (m_sceneMgr->hasManualObject(this->getID() + "_line" + std::to_string(i)))
            {
                ::Ogre::ManualObject* line =
                        m_sceneMgr->getManualObject(this->getID() + "_line" + std::to_string(i));
                ::Ogre::ManualObject* sphere = m_sceneMgr->getManualObject(
                            this->getID() + "_sphere" + "1" + std::to_string(i));
                ::Ogre::ManualObject* sphere2 = m_sceneMgr->getManualObject(
                            this->getID() + "_sphere" + "2" +std::to_string(i));
                destroyLabel(i);
                m_sceneMgr->destroyManualObject(sphere);
                m_sceneMgr->destroyManualObject(sphere2);
                m_sceneMgr->destroyManualObject(line);
            }
        }
    }
    _id = 0;
    m_labels.clear();
    m_millimeterValue.clear();
    m_labelNodes.clear();
    m_millimeterNodes.clear();
}

void SImageMultiDistances::removeDistance(::fwData::PointList::csptr plToRemove)
{
    static_cast<void>(plToRemove);
    removeAllDistance();
    this->updating();
}

void SImageMultiDistances::mouseMoveEvent(MouseButton button, int x, int y, int dx, int dy)
{
    static_cast<void>(dx);
    static_cast<void>(dy);
    if (button != MouseButton::LEFT || !m_activeInteraction) {
        return;
    }
    if (m_sceneMgr->hasManualObject(this->getID() + "_fake"))
    {
        ::Ogre::ManualObject* line = m_sceneMgr->getManualObject(this->getID() + "_fake");
        m_sceneMgr->destroyManualObject(line);
    }

    hideLine(_moveID);

    ::Ogre::ManualObject* m_line = m_sceneMgr->createManualObject(this->getID() + "_fake");

    // set the material
    ::fwData::Material::sptr m_material;
    m_material = ::fwData::Material::New();
    ::Ogre::ColourValue m_color(static_cast<float>(255) / 255.f, static_cast<float>(0) / 255.f, static_cast<float>(0) / 255.f, static_cast<float>(255) / 255.f);
    ::visuOgreAdaptor::SMaterial::sptr materialAdaptor = this->registerService< ::visuOgreAdaptor::SMaterial >(
                "::visuOgreAdaptor::SMaterial");
    materialAdaptor->registerInOut(m_material, ::visuOgreAdaptor::SMaterial::s_MATERIAL_INOUT, true);
    materialAdaptor->setID(this->getID() + materialAdaptor->getID());
    materialAdaptor->setMaterialName(this->getID() + materialAdaptor->getID());
    materialAdaptor->setRenderService( this->getRenderService() );
    materialAdaptor->setLayerID(m_layerID);
    materialAdaptor->setShadingMode("ambient");
    materialAdaptor->setMaterialTemplateName(::fwRenderOgre::Material::DEFAULT_MATERIAL_TEMPLATE_NAME);
    materialAdaptor->start();

    materialAdaptor->getMaterialFw()->setHasVertexColor(true);
    materialAdaptor->update();

    ::fwData::Mesh::PointValueType clickedPoint[3] {static_cast<float>(x), static_cast<float>(y), 0};

    /// Set the current cursor position to ogre position
    const ::Ogre::Vector3 worldspaceClikedPoint = ::fwRenderOgre::helper::Camera::convertPixelToViewSpace(
                *(this->getLayer()->getDefaultCamera()), clickedPoint);

    /// Draw the line
    m_line->begin(materialAdaptor->getMaterialName(), ::Ogre::RenderOperation::OT_LINE_LIST);
    if (_isBeginMove == 1) {
        m_line->position(static_cast<float>(_ps1[0]), static_cast<float>(_ps1[1]),
                static_cast<float>(1));
        m_line->colour(m_color);
        m_line->position(static_cast<float>(worldspaceClikedPoint[0]), static_cast<float>(worldspaceClikedPoint[1]),
                static_cast<float>(1));
    } else {
        m_line->position(static_cast<float>(worldspaceClikedPoint[0]), static_cast<float>(worldspaceClikedPoint[1]),
                static_cast<float>(1));
        m_line->colour(m_color);
        m_line->position(static_cast<float>(_ps2[0]), static_cast<float>(_ps2[1]),
                static_cast<float>(1));
    }
    m_line->end();
    m_rootSceneNode->attachObject(m_line);
}

void SImageMultiDistances::wheelEvent(int a, int b, int c)
{
    static_cast<void>(a);
    static_cast<void>(b);
    static_cast<void>(c);
}

void SImageMultiDistances::resizeEvent(int a, int b)
{
    static_cast<void>(a);
    static_cast<void>(b);
}

void SImageMultiDistances::keyPressEvent(int a)
{
    static_cast<void>(a);
}

void SImageMultiDistances::keyReleaseEvent(int a)
{
   static_cast<void>(a);
}

void SImageMultiDistances::buttonReleaseEvent(MouseButton button, int x, int y)
{
    if (button != MouseButton::LEFT || !m_activeInteraction) {
        return;
    }
    if (m_sceneMgr->hasManualObject(this->getID() + "_fake"))
    {
        ::Ogre::ManualObject* line = m_sceneMgr->getManualObject(this->getID() + "_fake");
        m_sceneMgr->destroyManualObject(line);
    }
    ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);
    ::fwData::Mesh::PointValueType clickedPoint[3] {static_cast<float>(x), static_cast<float>(y), 0};
    /// Set the current cursor position to ogre position
    const ::Ogre::Vector3 worldspaceClikedPoint = ::fwRenderOgre::helper::Camera::convertPixelToViewSpace(
                *(this->getLayer()->getDefaultCamera()), clickedPoint);

    ::fwData::Vector::sptr distanceField = image->getField< ::fwData::Vector >( ::fwDataTools::fieldHelper::Image::m_imageDistancesId);
    ::fwData::Vector::ContainerType& container = distanceField->getContainer();

    int i = 0;
    for(auto it = container.begin(); it != container.end() ; ++it)
    {
        if (i == _moveID) {
            ::fwData::PointList::sptr pointList = ::fwData::PointList::dynamicCast(*it);
            pointList->getPoints()[static_cast<size_t>(_isBeginMove)]->setCoord({static_cast<double>(worldspaceClikedPoint[0]), static_cast<double>(worldspaceClikedPoint[1])});
        }
        i++;
    }
    SLM_ASSERT("Missing image", image);
    auto sigImage = image->signal<::fwData::Image::ModifiedSignalType>(::fwData::Image::s_MODIFIED_SIG);
    sigImage->asyncEmit();
    this->updating();
}

bool SImageMultiDistances::matchPointToRemove(int toFind, int searchBigger)
{
    int searchSmaller = searchBigger;

    for (int i = 0; i < 2; i++) {
        if (toFind == searchSmaller || toFind == searchBigger) {
            return true;
        }
        searchBigger++;
        searchSmaller--;
    }
    return false;
}

bool SImageMultiDistances::checkMove(double ps1[3], double ps2[3], const ::Ogre::Vector3 worldspaceClikedPoint)
{
    int point1X = static_cast<int>(ps1[0]);
    int point1Y = static_cast<int>(ps1[1]);
    int point2X = static_cast<int>(ps2[0]);
    int point2Y = static_cast<int>(ps2[1]);
    int mouseClickX = static_cast<int>(worldspaceClikedPoint[0]);
    int mouseClickY = static_cast<int>(worldspaceClikedPoint[1]);

    if (matchPointToRemove(point1X, mouseClickX) == true && matchPointToRemove(point1Y, mouseClickY) == true) {
        _isBeginMove = 0;
        return true;
    } else if (matchPointToRemove(point2X, mouseClickX) == true && matchPointToRemove(point2Y, mouseClickY) == true) {
        _isBeginMove = 1;
        return true;
    }
    return false;
}

void SImageMultiDistances::buttonPressEvent(MouseButton button, int x, int y)
{
    if (button != MouseButton::LEFT) {
        return;
    }
    _moveID = 0;
    m_activeInteraction = false;
    //::fwData::Mesh::PointValueType clickedPoint[3] {static_cast<float>(x - 425), static_cast<float>(y - 425), 0};
    //je verifie si j'ai un point qui existe ici ou au alentour
    ::fwData::Mesh::PointValueType clickedPoint[3] {static_cast<float>(x), static_cast<float>(y), 0}; //mettre la taille de l'image
    const ::Ogre::Vector3 worldspaceClikedPoint = ::fwRenderOgre::helper::Camera::convertPixelToViewSpace(
                *(this->getLayer()->getDefaultCamera()), clickedPoint);


    std::cout << "ogre position[0]" << worldspaceClikedPoint[0] << std::endl;
    std::cout << "ogre position[1]" << worldspaceClikedPoint[1] << std::endl;
    ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);
    SLM_ASSERT("Missing image", image);
    ::fwData::Vector::sptr distanceField;
    distanceField = image->getField< ::fwData::Vector >( ::fwDataTools::fieldHelper::Image::m_imageDistancesId);
    //parcourir tout les points et voir si y'a un point qui est okay, stocker l'id et si c'est begin ou end
    int i = 0;
    for(::fwData::Object::sptr object :  *distanceField)
    {
        std::cout << "NEW OBJECT" <<std::endl;
        ::fwData::PointList::sptr distance    = ::fwData::PointList::dynamicCast(object);
        SLM_ASSERT( "Empty Point List for Distance !!!!", !distance->getPoints().empty() );
        m_point1 = distance->getPoints().front();
        m_point2 = distance->getPoints().back();
        ::fwData::Point::csptr p1 = m_point1.lock();
        ::fwData::Point::csptr p2 = m_point2.lock();

        double ps1[3];
        std::copy(p1->getCoord().begin(), (p1)->getCoord().end(), ps1 );
        double ps2[3];
        std::copy(p2->getCoord().begin(), (p2)->getCoord().end(), ps2 );
        if (checkMove(ps1, ps2, worldspaceClikedPoint) == true) {
            _moveID = i;
            //je sais qui je press donc je stock les coordonnes de ps1 et ps2
            std::copy(std::begin(ps1), std::end(ps1), _ps1 );
            std::copy(std::begin(ps2), std::end(ps2), _ps2 );
            m_activeInteraction = true;
        }
        i++;
    }
}

void SImageMultiDistances::focusInEvent()
{}

void SImageMultiDistances::focusOutEvent()
{}

//------------------------------------------------------------------------------

::fwServices::IService::KeyConnectionsMap SImageMultiDistances::getAutoConnections() const
{
    KeyConnectionsMap connections;

    connections.push(s_IMAGE_INOUT, ::fwData::Image::s_DISTANCE_ADDED_SIG, s_UPDATE_SLOT);
    connections.push(s_IMAGE_INOUT, ::fwData::Image::s_DISTANCE_REMOVED_SIG, s_REMOVE_DISTANCE_SLOT);
    connections.push(s_IMAGE_INOUT, ::fwData::Image::s_DISTANCE_DISPLAYED_SIG, s_UPDATE_SLOT);
    // connections.push(s_POINTLIST_INPUT, ::fwData::Object::s_MODIFIED_SIG, s_UPDATE_SLOT);
    return connections;
}

}
