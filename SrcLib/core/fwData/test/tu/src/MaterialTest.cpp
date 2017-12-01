/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "MaterialTest.hpp"

#include <fwData/Color.hpp>
#include <fwData/Material.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <exception>
#include <iostream>
#include <map>
#include <ostream>
#include <vector>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ::fwData::ut::MaterialTest );

namespace fwData
{
namespace ut
{

//------------------------------------------------------------------------------

void MaterialTest::setUp()
{
    // Set up context before running a test.

}
//------------------------------------------------------------------------------

void MaterialTest::tearDown()
{
    // Clean up after the test run.
}

//------------------------------------------------------------------------------

void MaterialTest::materialSetupTest()
{
    const int c_nbTextures = 8;
    std::ostringstream oss;

    //-----------test values
    ::fwData::Color::sptr CAMBIENT = ::fwData::Color::New();
    CAMBIENT->setRGBA(0.5f, 0.5f, 0.5f, 0.5f);

    ::fwData::Color::sptr CDIFF = ::fwData::Color::New();
    CDIFF->setRGBA(0.8f, 0.2f, 0.5f, 0.4f);

    ::fwData::Material::sptr material = ::fwData::Material::New();

    material->setAmbient(::fwData::Object::copy(CAMBIENT));
    material->setDiffuse(::fwData::Object::copy(CDIFF));

    CPPUNIT_ASSERT_EQUAL(material->ambient()->getRGBA()[0], CAMBIENT->getRGBA()[0]);
    CPPUNIT_ASSERT_EQUAL(material->diffuse()->getRGBA()[0], CDIFF->getRGBA()[0]);

    // Allocate and set the textures
    for(size_t i = 0; i < c_nbTextures; i++)
    {
        oss.str("");
        oss << i;
        material->addTexture(oss.str());
    }

    // Test if everything is OK
    CPPUNIT_ASSERT_EQUAL(material->ambient()->getRGBA()[0], CAMBIENT->getRGBA()[0]);
    CPPUNIT_ASSERT_EQUAL(material->diffuse()->getRGBA()[0], CDIFF->getRGBA()[0]);

    for(size_t i = 0; i < c_nbTextures; i++)
    {
        oss.str("");
        oss << i;
        CPPUNIT_ASSERT_EQUAL(material->getTexture(i), oss.str());
    }
}

//------------------------------------------------------------------------------

void MaterialTest::materialCopyTest()
{
    const int c_nbTextures = 8;
    std::ostringstream oss;

    //-----------test values
    ::fwData::Color::sptr CAMBIENT = ::fwData::Color::New();
    CAMBIENT->setRGBA(0.5f, 0.5f, 0.5f, 0.5f);

    ::fwData::Color::sptr CDIFF = ::fwData::Color::New();
    CDIFF->setRGBA(0.8f, 0.2f, 0.5f, 0.4f);

    ::fwData::Material::sptr material = ::fwData::Material::New();

    material->setAmbient(::fwData::Object::copy(CAMBIENT));
    material->setDiffuse(::fwData::Object::copy(CDIFF));

    CPPUNIT_ASSERT_EQUAL(material->ambient()->getRGBA()[0], CAMBIENT->getRGBA()[0]);
    CPPUNIT_ASSERT_EQUAL(material->diffuse()->getRGBA()[0], CDIFF->getRGBA()[0]);

    for(size_t i = 0; i < c_nbTextures; i++)
    {
        oss.str("");
        oss << i;
        material->addTexture(oss.str());
    }

    {
        // Check deep copy
        ::fwData::Material::csptr materialCopy = ::fwData::Object::copy(material);

        CPPUNIT_ASSERT(materialCopy);

        CPPUNIT_ASSERT_EQUAL(materialCopy->ambient()->getRGBA()[0], CAMBIENT->getRGBA()[0]);
        CPPUNIT_ASSERT_EQUAL(materialCopy->diffuse()->getRGBA()[0], CDIFF->getRGBA()[0]);

        for(size_t i = 0; i < c_nbTextures; i++)
        {
            oss.str("");
            oss << i;
            CPPUNIT_ASSERT_EQUAL(material->getTexture(i), oss.str());
        }
    }

    {
        // Check shallow copy
        ::fwData::Material::sptr materialCopy = ::fwData::Material::New();
        materialCopy->shallowCopy(material);

        CPPUNIT_ASSERT(materialCopy);

        CPPUNIT_ASSERT_EQUAL(materialCopy->ambient()->getRGBA()[0], CAMBIENT->getRGBA()[0]);
        CPPUNIT_ASSERT_EQUAL(materialCopy->diffuse()->getRGBA()[0], CDIFF->getRGBA()[0]);

        for(size_t i = 0; i < c_nbTextures; i++)
        {
            oss.str("");
            oss << i;
            CPPUNIT_ASSERT_EQUAL(material->getTexture(i), oss.str());
        }
    }
}

} //namespace ut
} //namespace fwData

