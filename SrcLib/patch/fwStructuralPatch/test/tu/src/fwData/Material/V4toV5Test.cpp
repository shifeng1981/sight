/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwData/Material/V4ToV5Test.hpp"

#include <fwStructuralPatch/fwData/Material/V4ToV5.hpp>

#include <fwAtoms/Base.hpp>
#include <fwAtoms/Boolean.hpp>
#include <fwAtoms/Map.hpp>
#include <fwAtoms/Numeric.hpp>
#include <fwAtoms/Numeric.hxx>
#include <fwAtoms/Object.hpp>
#include <fwAtoms/Object.hxx>
#include <fwAtoms/Sequence.hpp>
#include <fwAtoms/String.hpp>

#include <fwAtomsPatch/helper/functions.hpp>
#include <fwAtomsPatch/helper/Object.hpp>

#include <fwTest/Exception.hpp>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ::fwStructuralPatch::ut::fwData::Material::V4ToV5Test );

namespace fwStructuralPatch
{
namespace ut
{
namespace fwData
{
namespace Material
{

//------------------------------------------------------------------------------

void V4ToV5Test::setUp()
{
    // Set up context before running a test.
}

//------------------------------------------------------------------------------

void V4ToV5Test::tearDown()
{
    // Clean up after the test run.
}

//------------------------------------------------------------------------------

void V4ToV5Test::applyPatchTest()
{
    ::fwAtoms::Object::sptr matObjV4 = ::fwAtoms::Object::New();
    ::fwAtoms::Object::sptr matObjV5;

    ::fwAtomsPatch::helper::setClassname(matObjV4, "::fwData::Material");
    ::fwAtomsPatch::helper::setVersion(matObjV4, "4");

    ::fwAtomsPatch::helper::Object helper(matObjV4);

    ::fwAtoms::Sequence::sptr ambientSeq = ::fwAtoms::Sequence::New();
    ambientSeq->push_back(::fwAtoms::Numeric::New(1.f));
    ambientSeq->push_back(::fwAtoms::Numeric::New(0.5f));
    ambientSeq->push_back(::fwAtoms::Numeric::New(0.6f));
    ambientSeq->push_back(::fwAtoms::Numeric::New(0.12f));
    ::fwAtoms::Object::sptr ambient = ::fwAtoms::Object::New();
    ambient->setAttribute("rgba", ambientSeq);

    ::fwAtoms::Sequence::sptr diffuseSeq = ::fwAtoms::Sequence::New();
    diffuseSeq->push_back(::fwAtoms::Numeric::New(0.33f));
    diffuseSeq->push_back(::fwAtoms::Numeric::New(0.32f));
    diffuseSeq->push_back(::fwAtoms::Numeric::New(0.2f));
    diffuseSeq->push_back(::fwAtoms::Numeric::New(0.125f));
    ::fwAtoms::Object::sptr diffuse = ::fwAtoms::Object::New();
    diffuse->setAttribute("rgba", diffuseSeq);

    helper.addAttribute("shading_mode", ::fwAtoms::String::New("PHONG"));
    helper.addAttribute("representation_mode", ::fwAtoms::String::New("SURFACE"));
    helper.addAttribute("options_mode", ::fwAtoms::String::New("STANDARD"));
    helper.addAttribute("ambient", ambient);
    helper.addAttribute("diffuse", diffuse);
    helper.addAttribute("diffuse_texture", ::fwAtoms::Object::sptr());
    helper.addAttribute("diffuse_texture_filtering", ::fwAtoms::String::New("NEAREST"));
    helper.addAttribute("diffuse_texture_wrapping", ::fwAtoms::String::New("CLAMP"));
    helper.addAttribute("lighting", ::fwAtoms::Boolean::New(false));

    // Check that when converting a V4 Material object to a V5 Material object
    // We move from a single image member to a map of Texture objects
    {
        // Create an object and apply the patch
        matObjV5 = ::fwAtoms::Object::dynamicCast(matObjV4->clone());

        ::fwAtomsPatch::IPatch::NewVersionsType newVersions;
        newVersions[matObjV4] = matObjV5;

        ::fwStructuralPatch::fwData::Material::V4ToV5::sptr mat4ToMat5Patch;
        mat4ToMat5Patch = ::fwStructuralPatch::fwData::Material::V4ToV5::New();
        CPPUNIT_ASSERT_NO_THROW(mat4ToMat5Patch->apply(matObjV4, matObjV5, newVersions));

        CPPUNIT_ASSERT(matObjV5);
        // Try to get the map from the object
        ::fwAtoms::Map::sptr dtm = matObjV5->getAttribute< ::fwAtoms::Map>("diffuse_texture");
        /* Ensure that we have only 1 element in the map of textures for V5 */
        CPPUNIT_ASSERT(dtm->size() == 1);
        /* Ensure that this texture is named with the default name "diffuse" */
        CPPUNIT_ASSERT(dtm->find("diffuse") != dtm->end());

        /* Get the default texture and test if the members are available */
        ::fwAtoms::Object::sptr dt = ::fwAtoms::Object::dynamicCast(dtm->find("diffuse")->second);
        // The image is empty at initialization, so we don't check it
        CPPUNIT_ASSERT(dt->getAttribute< ::fwAtoms::String>("filtering"));
        CPPUNIT_ASSERT(dt->getAttribute< ::fwAtoms::String>("wrapping"));
        CPPUNIT_ASSERT(dt->getAttribute< ::fwAtoms::String>("blending"));
    }
}

//------------------------------------------------------------------------------
} //namespace Material
} //namespace fwData
} //namespace ut
} //namespace fwStructuralPatch
