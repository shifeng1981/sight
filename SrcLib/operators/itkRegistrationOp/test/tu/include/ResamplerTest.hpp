/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2017-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include <cppunit/extensions/HelperMacros.h>

namespace itkRegistrationOp
{
namespace ut
{

class ResamplerTest : public CPPUNIT_NS::TestFixture
{
CPPUNIT_TEST_SUITE( ResamplerTest );
CPPUNIT_TEST( identityTest );
CPPUNIT_TEST( translateTest );
//CPPUNIT_TEST( rotateTest );//fail
CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void identityTest();
    void translateTest();
    void rotateTest();

};

} //namespace ut
} //namespace itkRegistrationOp
