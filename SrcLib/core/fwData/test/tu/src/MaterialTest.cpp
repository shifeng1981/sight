/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
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

    ::fwData::Image::sptr img;

    const size_t nDim = 2;
    ::fwTools::Type type = ::fwTools::Type::create("int8");
    double spacing = 2.5;
    std::vector<double> vectorSpacing(nDim, spacing);
    double origin = 2.7;
    std::vector<double> vectorOrigin(nDim, origin);
    ::boost::int32_t size = 42;
    ::fwData::Image::SizeType vectorSize(nDim, size);

    std::ostringstream oss;
    for(int i = 0; i < c_nbTextures; i++)
    {
        img = ::fwData::Image::New();

        img->setType(::fwTools::Type::create("int8"));
        img->setSpacing(vectorSpacing);
        img->setOrigin(vectorOrigin);
        img->setSize(vectorSize);

        oss.str("");
        oss << "test" << i << std::endl;
        material->setTexture(img, oss.str());
        material->setTextureFiltering(::fwData::Texture::FilteringType::LINEAR, oss.str());
        material->setTextureWrapping(::fwData::Texture::WrappingType::REPEAT, oss.str());
        material->setTextureBlending(::fwData::Texture::BlendingType::MODULATE, oss.str());
    }

    // Test if everything is OK
    CPPUNIT_ASSERT_EQUAL(material->ambient()->getRGBA()[0], CAMBIENT->getRGBA()[0]);
    CPPUNIT_ASSERT_EQUAL(material->diffuse()->getRGBA()[0], CDIFF->getRGBA()[0]);

    for(int i = 0; i < c_nbTextures; i++)
    {
        oss.str("");
        oss << "test" << i << std::endl;

        CPPUNIT_ASSERT_EQUAL(material->getTextureFiltering(
                                 oss.str()), ::fwData::Texture::FilteringType::LINEAR);
        CPPUNIT_ASSERT_EQUAL(material->getTextureWrapping(
                                 oss.str()), ::fwData::Texture::WrappingType::REPEAT);
        CPPUNIT_ASSERT_EQUAL(material->getTextureBlending(
                                 oss.str()), ::fwData::Texture::BlendingType::MODULATE);

        img = material->getTexture(oss.str());
        CPPUNIT_ASSERT(img != nullptr);
        CPPUNIT_ASSERT_EQUAL(img->getNumberOfDimensions(), nDim);
        CPPUNIT_ASSERT(img->getType() == type);
        CPPUNIT_ASSERT(img->getSpacing() == vectorSpacing);
        CPPUNIT_ASSERT(img->getOrigin() == vectorOrigin);
        CPPUNIT_ASSERT(img->getSize() == vectorSize);
    }
}

//------------------------------------------------------------------------------

void MaterialTest::materialCopyTest()
{
    const int c_nbTextures = 8;

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

    ::fwData::Image::sptr img;

    const size_t nDim = 2;
    ::fwTools::Type type = ::fwTools::Type::create("int8");
    double spacing = 2.5;
    std::vector<double> vectorSpacing(nDim, spacing);
    double origin = 2.7;
    std::vector<double> vectorOrigin(nDim, origin);
    ::boost::int32_t size = 42;
    ::fwData::Image::SizeType vectorSize(nDim, size);

    std::ostringstream oss;
    for(int i = 0; i < c_nbTextures; i++)
    {
        img = ::fwData::Image::New();

        img->setType(::fwTools::Type::create("int8"));
        img->setSpacing(vectorSpacing);
        img->setOrigin(vectorOrigin);
        img->setSize(vectorSize);

        oss.str("");
        oss << "test" << i << std::endl;
        material->setTexture(img, oss.str());
        material->setTextureFiltering(::fwData::Texture::FilteringType::LINEAR, oss.str());
        material->setTextureWrapping(::fwData::Texture::WrappingType::REPEAT, oss.str());
        material->setTextureBlending(::fwData::Texture::BlendingType::MODULATE, oss.str());
    }

    {
        // Check deep copy
        ::fwData::Material::csptr materialCopy = ::fwData::Object::copy(material);

        CPPUNIT_ASSERT(materialCopy);

        CPPUNIT_ASSERT_EQUAL(materialCopy->ambient()->getRGBA()[0], CAMBIENT->getRGBA()[0]);
        CPPUNIT_ASSERT_EQUAL(materialCopy->diffuse()->getRGBA()[0], CDIFF->getRGBA()[0]);

        for(int i = 0; i < c_nbTextures; i++)
        {
            oss.str("");
            oss << "test" << i << std::endl;

            CPPUNIT_ASSERT_EQUAL(materialCopy->getTextureFiltering(
                                     oss.str()), ::fwData::Texture::FilteringType::LINEAR);
            CPPUNIT_ASSERT_EQUAL(materialCopy->getTextureWrapping(
                                     oss.str()), ::fwData::Texture::WrappingType::REPEAT);
            CPPUNIT_ASSERT_EQUAL(materialCopy->getTextureBlending(
                                     oss.str()), ::fwData::Texture::BlendingType::MODULATE);

            img = materialCopy->getTexture(oss.str());
            CPPUNIT_ASSERT(img != nullptr);
            CPPUNIT_ASSERT_EQUAL(img->getNumberOfDimensions(), nDim);
            CPPUNIT_ASSERT(img->getType() == type);
            CPPUNIT_ASSERT(img->getSpacing() == vectorSpacing);
            CPPUNIT_ASSERT(img->getOrigin() == vectorOrigin);
            CPPUNIT_ASSERT(img->getSize() == vectorSize);
        }
    }

    {
        // Check shallow copy
        ::fwData::Material::sptr materialCopy = ::fwData::Material::New();
        materialCopy->shallowCopy(material);

        CPPUNIT_ASSERT(materialCopy);

        CPPUNIT_ASSERT_EQUAL(materialCopy->ambient()->getRGBA()[0], CAMBIENT->getRGBA()[0]);
        CPPUNIT_ASSERT_EQUAL(materialCopy->diffuse()->getRGBA()[0], CDIFF->getRGBA()[0]);

        for(int i = 0; i < c_nbTextures; i++)
        {
            oss.str("");
            oss << "test" << i << std::endl;

            CPPUNIT_ASSERT_EQUAL(materialCopy->getTextureFiltering(
                                     oss.str()), ::fwData::Texture::FilteringType::LINEAR);
            CPPUNIT_ASSERT_EQUAL(materialCopy->getTextureWrapping(
                                     oss.str()), ::fwData::Texture::WrappingType::REPEAT);
            CPPUNIT_ASSERT_EQUAL(materialCopy->getTextureBlending(
                                     oss.str()), ::fwData::Texture::BlendingType::MODULATE);

            img = materialCopy->getTexture(oss.str());
            CPPUNIT_ASSERT(img != nullptr);
            CPPUNIT_ASSERT_EQUAL(img->getNumberOfDimensions(), nDim);
            CPPUNIT_ASSERT(img->getType() == type);
            CPPUNIT_ASSERT(img->getSpacing() == vectorSpacing);
            CPPUNIT_ASSERT(img->getOrigin() == vectorOrigin);
            CPPUNIT_ASSERT(img->getSize() == vectorSize);
        }
    }
}

} //namespace ut
} //namespace fwData

