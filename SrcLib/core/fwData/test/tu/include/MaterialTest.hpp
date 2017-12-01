/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDATA_UT_MATERIALTEST_HPP__
#define __FWDATA_UT_MATERIALTEST_HPP__

#include <cppunit/extensions/HelperMacros.h>

namespace fwData
{
namespace ut
{

class MaterialTest : public CPPUNIT_NS::TestFixture
{
private:
    CPPUNIT_TEST_SUITE( MaterialTest );
    CPPUNIT_TEST( materialSetupTest );
    CPPUNIT_TEST( materialCopyTest );
    CPPUNIT_TEST_SUITE_END();

public:
    // interface
    void setUp();
    void tearDown();

    void materialSetupTest();
    void materialCopyTest();
};
} //namespace ut
} //namespace fwData
#endif //__FWDATA_UT_MATERIALTEST_HPP__
