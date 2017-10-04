/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "TextureTest.hpp"

#include <fwData/Texture.hpp>

#include <vector>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ::fwData::ut::TextureTest );

namespace fwData
{
namespace ut
{

//------------------------------------------------------------------------------

void TextureTest::setUp()
{
    // Set up context before running a test.

}
//------------------------------------------------------------------------------

void TextureTest::tearDown()
{
    // Clean up after the test run.
}

//------------------------------------------------------------------------------

void TextureTest::textureSetupTest()
{
    //-----------test values
    ::fwData::Image::sptr img = ::fwData::Image::New();

    const size_t nDim = 2;
    ::fwTools::Type type = ::fwTools::Type::create("int8");
    double spacing = 2.5;
    std::vector<double> vectorSpacing(nDim, spacing);
    double origin = 2.7;
    std::vector<double> vectorOrigin(nDim, origin);
    ::boost::int32_t size = 42;
    ::fwData::Image::SizeType vectorSize(nDim, size);

    img->setType(::fwTools::Type::create("int8"));
    img->setSpacing(vectorSpacing);
    img->setOrigin(vectorOrigin);
    img->setSize(vectorSize);

    ::fwData::Texture::sptr texture = ::fwData::Texture::New();

    texture->setFiltering(::fwData::Texture::FilteringType::LINEAR);
    texture->setWrapping(::fwData::Texture::WrappingType::REPEAT);
    texture->setBlending(::fwData::Texture::BlendingType::MODULATE);

    texture->setImage(img);

    // Check texture parameters
    {
        CPPUNIT_ASSERT_EQUAL(texture->getFiltering(), ::fwData::Texture::FilteringType::LINEAR);
        CPPUNIT_ASSERT_EQUAL(texture->getWrapping(), ::fwData::Texture::WrappingType::REPEAT);
        CPPUNIT_ASSERT_EQUAL(texture->getBlending(), ::fwData::Texture::BlendingType::MODULATE);

        CPPUNIT_ASSERT(texture->getImage() != nullptr);

        CPPUNIT_ASSERT_EQUAL(texture->getImage()->getNumberOfDimensions(), nDim);
        CPPUNIT_ASSERT(texture->getImage()->getType() == type);
        CPPUNIT_ASSERT(texture->getImage()->getSpacing() == vectorSpacing);
        CPPUNIT_ASSERT(texture->getImage()->getOrigin() == vectorOrigin);
        CPPUNIT_ASSERT(texture->getImage()->getSize() == vectorSize);
    }

    {
        // Check deep copy
        ::fwData::Texture::csptr textureCopy = ::fwData::Object::copy(texture);
        CPPUNIT_ASSERT(textureCopy);

        CPPUNIT_ASSERT_EQUAL(textureCopy->getFiltering(), ::fwData::Texture::FilteringType::LINEAR);
        CPPUNIT_ASSERT_EQUAL(textureCopy->getWrapping(), ::fwData::Texture::WrappingType::REPEAT);
        CPPUNIT_ASSERT_EQUAL(textureCopy->getBlending(), ::fwData::Texture::BlendingType::MODULATE);

        CPPUNIT_ASSERT(textureCopy->getImage() != nullptr);

        CPPUNIT_ASSERT_EQUAL(textureCopy->getImage()->getNumberOfDimensions(), nDim);
        CPPUNIT_ASSERT(textureCopy->getImage()->getType() == type);
        CPPUNIT_ASSERT(textureCopy->getImage()->getSpacing() == vectorSpacing);
        CPPUNIT_ASSERT(textureCopy->getImage()->getOrigin() == vectorOrigin);
        CPPUNIT_ASSERT(textureCopy->getImage()->getSize() == vectorSize);
    }

    {
        // Check shallow copy
        ::fwData::Texture::sptr textureCopy = ::fwData::Texture::New();
        textureCopy->shallowCopy(texture);
        CPPUNIT_ASSERT(textureCopy);

        CPPUNIT_ASSERT_EQUAL(textureCopy->getFiltering(), ::fwData::Texture::FilteringType::LINEAR);
        CPPUNIT_ASSERT_EQUAL(textureCopy->getWrapping(), ::fwData::Texture::WrappingType::REPEAT);
        CPPUNIT_ASSERT_EQUAL(textureCopy->getBlending(), ::fwData::Texture::BlendingType::MODULATE);

        CPPUNIT_ASSERT(textureCopy->getImage() != nullptr);

        CPPUNIT_ASSERT_EQUAL(textureCopy->getImage()->getNumberOfDimensions(), nDim);
        CPPUNIT_ASSERT(textureCopy->getImage()->getType() == type);
        CPPUNIT_ASSERT(textureCopy->getImage()->getSpacing() == vectorSpacing);
        CPPUNIT_ASSERT(textureCopy->getImage()->getOrigin() == vectorOrigin);
        CPPUNIT_ASSERT(textureCopy->getImage()->getSize() == vectorSize);
    }
}

} //namespace ut
} //namespace fwData

