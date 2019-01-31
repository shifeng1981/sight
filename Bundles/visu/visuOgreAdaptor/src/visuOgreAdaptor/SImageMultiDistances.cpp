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
#include <fwData/PointList.hpp>
#include <fwData/String.hpp>
#include <fwData/Vector.hpp>

#include <fwDataTools/fieldHelper/Image.hpp>

#include <fwRenderOgre/helper/Font.hpp>
#include <fwRenderOgre/helper/ManualObject.hpp>
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
    m_sceneMgr      = this->getSceneManager();
    m_rootSceneNode = this->getSceneManager()->getRootSceneNode();
    this->updating();
}

//------------------------------------------------------------------------------

void SImageMultiDistances::updating()
{
    // ::fwData::PointList::csptr ptList = this->getInput< ::fwData::PointList >(s_POINTLIST_INPUT);
    // get PointList in image Field then install distance service if required
    this->requestRender();
    ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);
    SLM_ASSERT("Missing image", image);

    ::fwData::Vector::sptr distanceField;
    distanceField = image->getField< ::fwData::Vector >( ::fwDataTools::fieldHelper::Image::m_imageDistancesId);
    removeAllDistance();
    if( distanceField )
    {
        m_distanceField = distanceField;
        for(::fwData::Object::sptr object :  *distanceField)
        {
            ::fwData::PointList::sptr distance    = ::fwData::PointList::dynamicCast(object);
            ::fwData::String::sptr relatedService = distance->getField< ::fwData::String >(
                ::fwDataTools::fieldHelper::Image::m_relatedServiceId);

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

//je recupere des coordonnées, j'affiche un trait mais il est pas juste (pas les bonnes coordonnées ?).
//A quoi correspond le troisieme tableau? et trouver une technique pour deplacer le trait, puis faire plusieurs trait

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
    m_millimeterNodes[static_cast<size_t>(_id)]->translate(static_cast<float>(coord[0] / 5),
                                                           static_cast<float>(coord[1] / 5 + (_id * 8)),
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

    std::cout << "id to create = " << labelNumber << std::endl;
    m_labels.push_back(::fwRenderOgre::Text::New(this->getID() + labelNumber, m_sceneMgr, textContainer,
                                                 dejaVuSansFont, cam));
    m_labels[static_cast<size_t>(_id)]->setText(labelNumber);
    m_labels[static_cast<size_t>(_id)]->setCharHeight(static_cast<float>(0.03));
    m_labels[static_cast<size_t>(_id)]->setTextColor(::Ogre::ColourValue(0, 0, 255));
    m_labelNodes.push_back(m_rootSceneNode->createChildSceneNode(this->getID() + "_id" + labelNumber));
    m_labelNodes[static_cast<size_t>(_id)]->attachObject(m_labels[static_cast<size_t>(_id)]);
    ::fwData::Point::PointCoordArrayType coord = point->getCoord();
    m_labelNodes[static_cast<size_t>(_id)]->translate(static_cast<float>(coord[0]),
                                                      static_cast<float>(coord[1] + (_id * 8)),
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

        Ogre::Vector3 a = Ogre::Vector3(static_cast<Ogre::Real>(ps1[0] / 5), static_cast<Ogre::Real>(ps1[1] / 5), 0);
        Ogre::Vector3 b = Ogre::Vector3(static_cast<Ogre::Real>(ps2[0] / 5), static_cast<Ogre::Real>(ps2[1] / 5), 0);

        if (m_sceneMgr->hasManualObject(this->getID() + "_line" + std::to_string(_id)))
        {
            std::cout << "j'existe deja est je suis le = " << _id << std::endl;
        }

        ::Ogre::ManualObject* m_line = m_sceneMgr->createManualObject(this->getID() + "_line" + std::to_string(_id));
        // set the material
        ::fwData::Material::sptr m_material;
        m_material = ::fwData::Material::New();
        ::Ogre::ColourValue m_color;

        m_color.r                                          = static_cast<float>(0) / 255.f;
        m_color.g                                          = static_cast<float>(0) / 255.f;
        m_color.b                                          = static_cast<float>(255) / 255.f;
        m_color.a                                          = static_cast<float>(255) / 255.f;
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

        // j'ai un point, je doit donnner la position exact et trouver un bon id pour mes deux points
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

        // Draw
        m_line->begin(materialAdaptor->getMaterialName(), ::Ogre::RenderOperation::OT_LINE_LIST);
        m_line->position(static_cast<float>(ps1[0] / 5), static_cast<float>(ps1[1] / 5 + (_id * 8)),
                         static_cast<float>(0.01));
        m_line->colour(m_color);
        m_line->position(static_cast<float>(ps2[0] / 5), static_cast<float>(ps2[1] / 5 + (_id * 8)),
                         static_cast<float>(0.01));
        m_line->colour(m_color);
        m_line->end();

        createLabel(p1);
        affMillimeter(p2, getDistance(a, b));
        this->attachNode(m_line);

        m_pointNode =
            this->m_rootSceneNode->createChildSceneNode(this->getID() + "_point" + "1" + std::to_string(_id));
        m_pointNode2 =
            this->m_rootSceneNode->createChildSceneNode(this->getID() + "_point" + "2" + std::to_string(_id));

        m_pointNode->attachObject(sphere);
        m_pointNode->translate(static_cast<float>(ps1[0] / 5), static_cast<float>(ps1[1] / 5 + (_id * 8)), 0);
        m_pointNode2->attachObject(sphere2);
        m_pointNode2->translate(static_cast<float>(ps2[0] / 5), static_cast<float>(ps2[1] / 5 + (_id * 8)), 0);
    }
}

//------------------------------------------------------------------------------

void SImageMultiDistances::attachNode(::Ogre::ManualObject* _node)
{
    m_rootSceneNode->attachObject(_node);
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

void SImageMultiDistances::removeAllDistance()
{
    ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);
    SLM_ASSERT("Missing image", image);
    ::fwData::Vector::sptr distanceField;
    distanceField = image->getField< ::fwData::Vector >( ::fwDataTools::fieldHelper::Image::m_imageDistancesId);

    if (!distanceField)
    {
        distanceField = m_distanceField;
    }
    if( distanceField )
    {
        std::cout << "mon id = " << _id << std::endl;
        for (int i = 0; i < _id; i++)
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

//Pour aujourd"hui: il faut que j'affiche les infos du points a remove pour trouver tout ce que je dois remove
//Je dois faire les points sur chaque extremités (regarder createsphre)
//Puis m'inspirer de VRWidget pour les faires moves
void SImageMultiDistances::removeDistance(::fwData::PointList::csptr plToRemove)
{
    (void)plToRemove;
    removeAllDistance();
    this->updating();
}

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
