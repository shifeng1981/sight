/************************************************************************
 *
 * Copyright (C) 2018 IRCAD France
 * Copyright (C) 2018 IHU Strasbourg
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

#include "CameraTest.hpp"

#include "arData/Camera.hpp"

#include "fwRenderOgre/Utils.hpp"
#include <fwRenderOgre/helper/Camera.hpp>

#include <OgreLogManager.h>
#include <OgreMatrix4.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>

#include <limits>
#include <tuple>
#include <vector>

CPPUNIT_TEST_SUITE_REGISTRATION( ::fwRenderOgre::ut::CameraTest );

namespace fwRenderOgre
{

namespace ut
{

//------------------------------------------------------------------------------

void CameraTest::setUp()
{
}

//------------------------------------------------------------------------------

void CameraTest::tearDown()
{
}

//------------------------------------------------------------------------------

void compareMatrix(const ::Ogre::Matrix4& _m1, const ::Ogre::Matrix4& _m2)
{
    for(unsigned int i = 0; i < 4; ++i)
    {
        for(unsigned int j = 0; j < 4; ++j)
        {
            CPPUNIT_ASSERT_DOUBLES_EQUAL(_m1[i][j], _m2[i][j], 0.00000000000001);
        }
    }
}

//------------------------------------------------------------------------------

void comparePoint(const Ogre::Vector3& _p1, const Ogre::Vector3& _p2)
{
    CPPUNIT_ASSERT_DOUBLES_EQUAL(_p1[0], _p2[0], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(_p1[1], _p2[1], 0.00001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(_p1[2], _p2[2], 0.00001);
}

//------------------------------------------------------------------------------

void CameraTest::computeProjectionMatrix()
{
    // Sorry for this incomprehensible test but this is better than 16 loops
    const double cx     = 988.898;
    const double cy     = 553.769;
    const double fx     = 1590.59;
    const double fy     = 1628.71;
    const size_t width  = 1920;
    const size_t height = 1080;
    const float n       = 0.1f;
    const float f       = 100;

    // Original matrix
    ::Ogre::Matrix4 expected(1.6568645238876f,  0.f,    -0.0295966863632202f,   0.f,
                             0.f,   3.01612949371338f,  0.0245949625968933f,    0.f,
                             0.f,   0.f,    -1.00200200080872f, -0.200200200080872f,
                             0.f,   0.f,    -1.f,   0.f);

    // Original camera
    ::arData::Camera::sptr camera = ::arData::Camera::New();
    camera->setCx(cx);
    camera->setCy(cy);
    camera->setFx(fx);
    camera->setFy(fy);
    camera->setWidth(width);
    camera->setHeight(height);

    {
        // Function of the camera to set, followed by the new value in the expected matrix
        typedef std::tuple< std::function< void (const arData::Camera::sptr, double) >, double, unsigned int,
                            unsigned int, float> tupleType;
        const std::vector<tupleType> permutation{
            tupleType(&::arData::Camera::setCx, cx, 0, 2, -0.0295966863632202f),
            tupleType(&::arData::Camera::setCy, cy, 1, 2, 0.0245949625968933f),
            tupleType(&::arData::Camera::setFx, fx, 0, 0, 1.65686452388763f),
            tupleType(&::arData::Camera::setFy, fy, 1, 1, 3.01612949371338f),

            tupleType(&::arData::Camera::setCx, 0., 0, 2, 1.00104212760925f),
            tupleType(&::arData::Camera::setCy, 0., 1, 2, -1.00185346603394f),
            tupleType(&::arData::Camera::setFx, 0., 0, 0, 0),
            tupleType(&::arData::Camera::setFy, 0., 1, 1, 0),

            tupleType(&::arData::Camera::setCx, std::numeric_limits<float>::min(), 0, 2, 1.00104212760925f),
            tupleType(&::arData::Camera::setCy, std::numeric_limits<float>::min(), 1, 2, -1.00185346603394f),
            tupleType(&::arData::Camera::setFx, std::numeric_limits<float>::min(), 0, 0, 1.22445459812703e-41f),
            tupleType(&::arData::Camera::setFy, std::numeric_limits<float>::min(), 1, 1, 2.17677703448217e-41f),

            tupleType(&::arData::Camera::setCx, std::numeric_limits<float>::max(), 0, 2, -3.5464549820937e+35f),
            tupleType(&::arData::Camera::setCy, std::numeric_limits<float>::max(), 1, 2, 6.30736510970334e+35f),
            tupleType(&::arData::Camera::setFx,
                      std::numeric_limits<float>::max(), 0, 0, std::numeric_limits<float>::infinity()),
            tupleType(&::arData::Camera::setFy,
                      std::numeric_limits<float>::max(), 1, 1, std::numeric_limits<float>::infinity()),
        };

        // Apply permutations and check the expected result
        for(auto tu : permutation)
        {
            auto fun = std::get<0>(tu);
            // Apply function on the camera
            fun(camera, std::get<1>(tu));
            // Change the expected result
            expected[std::get<2>(tu)][std::get<3>(tu)] = std::get<4>(tu);

            ::Ogre::Matrix4 actual =
                ::fwRenderOgre::helper::Camera::computeProjectionMatrix(    *camera,
                                                                            static_cast< float >(camera->getWidth()),
                                                                            static_cast< float >(camera->getHeight()),
                                                                            n,
                                                                            f);
            compareMatrix(expected, actual);
        }
    }

    {
        // Function of the camera to set, followed by the new value in the expected matrix
        typedef std::tuple<std::function< void (const arData::Camera::sptr, size_t) >, size_t, unsigned int,
                           unsigned int, float> tupleType;
        const std::vector<tupleType> permutation{
            tupleType(&::arData::Camera::setWidth, width, 0, 2, -3.5464549820937e+35),
            tupleType(&::arData::Camera::setHeight, height, 1, 2, 6.30736510970334e+35),

            tupleType(&::arData::Camera::setWidth, 1, 0, 2, -std::numeric_limits<float>::infinity()),
            tupleType(&::arData::Camera::setHeight, 1, 1, 2, std::numeric_limits<float>::infinity()),

            tupleType(&::arData::Camera::setWidth, std::numeric_limits<size_t>::max(), 0, 2, -3.68934859483958e+19f),
            tupleType(&::arData::Camera::setHeight, std::numeric_limits<size_t>::max(), 1, 2, 3.68934859483958e+19f),
        };

        // Apply permutations and check the expected result
        for(auto tu : permutation)
        {
            auto fun = std::get<0>(tu);
            // Apply function on the camera
            fun(camera, std::get<1>(tu));
            // Change the expected result
            expected[std::get<2>(tu)][std::get<3>(tu)] = std::get<4>(tu);

            ::Ogre::Matrix4 actual =
                ::fwRenderOgre::helper::Camera::computeProjectionMatrix(    *camera,
                                                                            static_cast< float >(camera->getWidth()),
                                                                            static_cast< float >(camera->getHeight()),
                                                                            n,
                                                                            f);
            compareMatrix(expected, actual);
        }
    }
}

//------------------------------------------------------------------------------

void CameraTest::convertPixelToWorldSpace()
{
    const size_t width  = 1920;
    const size_t height = 1080;
    const double cx     = static_cast<double>(width)/2.;
    const double cy     = static_cast<double>(height)/2.;
    const double fx     = 10;
    const double fy     = 10;
    const float n       = 0.1f;
    const float f       = 100;

    // Original camera
    ::arData::Camera::sptr camera = ::arData::Camera::New();
    camera->setCx(cx);
    camera->setCy(cy);
    camera->setFx(fx);
    camera->setFy(fy);
    camera->setWidth(width);
    camera->setHeight(height);

    ::Ogre::Camera* ogreCamera;

    auto root = ::fwRenderOgre::Utils::getOgreRoot();

    auto ogreRenderWindow = root->createRenderWindow("Dummy-RenderWindow",
                                                     static_cast<unsigned int>(1),
                                                     static_cast<unsigned int>(1),
                                                     false,
                                                     nullptr);

    ogreRenderWindow->setVisible(false);

    ogreRenderWindow->setAutoUpdated(false);

    auto sceneManager = root->createSceneManager("DefaultSceneManager", "Testing");

    ogreCamera = sceneManager->createCamera("DefaultCam");

    auto viewport = ogreRenderWindow->addViewport(ogreCamera, 0);

    float point1[3] = {0, 0, 0};
    float point2[3] = {12, 34, 56};
    float point3[3] = {-65, -43, -21};

    Ogre::Vector3 point1Expected = {0, 0, 0};
    Ogre::Vector3 point2Expected = {0, 0, 56};
    Ogre::Vector3 point3Expected = {0, 0, -21};

    const float actualWidth  = static_cast<float>(viewport->getActualWidth());
    const float actualHeight = static_cast<float>(viewport->getActualHeight());

    // Since the viewport's dimensions are relative to each configuration, we must recalculate all conversions.
    point1Expected[0] = point1[0]/actualWidth *2.f - 1.f;
    point1Expected[1] = -(point1[1]/actualHeight * 2.f - 1.f);

    point2Expected[0] = point2[0]/actualWidth *2.f - 1.f;
    point2Expected[1] = -(point2[1]/actualHeight * 2.f - 1.f);

    point3Expected[0] = point3[0]/actualWidth *2.f - 1.f;
    point3Expected[1] = -(point3[1]/actualHeight * 2.f - 1.f);

    const ::Ogre::Matrix4 viewMat = ogreCamera->getViewMatrix();
    const ::Ogre::Matrix4 projMat = ogreCamera->getProjectionMatrixWithRSDepth();

    const auto inversedCombinedMat = (projMat * viewMat).inverse();

    point1Expected = inversedCombinedMat * point1Expected;
    point2Expected = inversedCombinedMat * point2Expected;
    point3Expected = inversedCombinedMat * point3Expected;

    const auto result1 = ::fwRenderOgre::helper::Camera::convertPixelToViewSpace(*ogreCamera, point1);
    const auto result2 = ::fwRenderOgre::helper::Camera::convertPixelToViewSpace(*ogreCamera, point2);
    const auto result3 = ::fwRenderOgre::helper::Camera::convertPixelToViewSpace(*ogreCamera, point3);

    Utils::destroyOgreRoot();

    comparePoint(result1, point1Expected);
    comparePoint(result2, point2Expected);
    comparePoint(result3, point3Expected);
}

//------------------------------------------------------------------------------

} //namespace ut
} //namespace fwRenderOgre
