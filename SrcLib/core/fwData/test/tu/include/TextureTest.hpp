/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDATA_UT_TEXTURETEST_HPP__
#define __FWDATA_UT_TEXTURETEST_HPP__

#include <cppunit/extensions/HelperMacros.h>

namespace fwData
{
namespace ut
{

class TextureTest : public CPPUNIT_NS::TestFixture
{
private:
    CPPUNIT_TEST_SUITE( TextureTest );
    CPPUNIT_TEST( textureSetupTest );
    CPPUNIT_TEST_SUITE_END();

public:
    // interface
    void setUp();
    void tearDown();

    void textureSetupTest();
};
} //namespace ut
} //namespace fwData
#endif //__FWDATA_UT_TEXTURETEST_HPP__
