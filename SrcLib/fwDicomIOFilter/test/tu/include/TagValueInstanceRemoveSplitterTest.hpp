/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDICOMIOFILTER_UT_TAGVALUEINSTANCEREMOVESPLITTERTEST_HPP__
#define __FWDICOMIOFILTER_UT_TAGVALUEINSTANCEREMOVESPLITTERTEST_HPP__

#include <fwServices/macros.hpp>
#include <fwRuntime/EConfigurationElement.hpp>

#include <cppunit/extensions/HelperMacros.h>

namespace fwDicomIOFilter
{
namespace ut
{

/**
 * @class TagValueInstanceRemoveSplitterTest
 * @brief Test TagValueInstanceRemoveSplitter class
 */
class TagValueInstanceRemoveSplitterTest : public CPPUNIT_NS::TestFixture
{
CPPUNIT_TEST_SUITE( TagValueInstanceRemoveSplitterTest );
CPPUNIT_TEST( simpleApplication );
CPPUNIT_TEST_SUITE_END();

public:
    // interface
    void setUp();
    void tearDown();

    /// Apply the patch and verify that the DicomSeries has been correctly modified
    void simpleApplication();

};

} // namespace ut
} // namespace fwDicomIOFilter

#endif // __FWDICOMIOFILTER_UT_TAGVALUEINSTANCEREMOVESPLITTERTEST_HPP__
