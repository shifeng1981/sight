/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwData/Material/V5ToV4Test.hpp"

#include <fwStructuralPatch/fwData/Material/V5ToV4.hpp>

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
CPPUNIT_TEST_SUITE_REGISTRATION( ::fwStructuralPatch::ut::fwData::Material::V5ToV4Test );

namespace fwStructuralPatch
{
namespace ut
{
namespace fwData
{
namespace Material
{

//------------------------------------------------------------------------------

void V5ToV4Test::setUp()
{
    // Set up context before running a test.
}

//------------------------------------------------------------------------------

void V5ToV4Test::tearDown()
{
    // Clean up after the test run.
}

//------------------------------------------------------------------------------

void V5ToV4Test::applyPatchTest()
{
    ::fwAtoms::Object::sptr matObjV5 = ::fwAtoms::Object::New();
    ::fwAtoms::Object::sptr matObjV4;

    ::fwAtomsPatch::helper::setClassname(matObjV5, "::fwData::Material");
    ::fwAtomsPatch::helper::setVersion(matObjV5, "5");

    ::fwAtomsPatch::helper::Object helper(matObjV5);

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
    helper.addAttribute("lighting", ::fwAtoms::Boolean::New(false));

    // Initialize a map with a dummy texture
    ::fwAtoms::Map::sptr dtm   = ::fwAtoms::Map::New();
    ::fwAtoms::Object::sptr dt = ::fwAtoms::Object::New();

    ::fwAtomsPatch::helper::Object helperDT(dt);
    helperDT.addAttribute("image", ::fwAtoms::Object::New());
    helperDT.addAttribute("filtering", ::fwAtoms::String::New("NEAREST"));
    helperDT.addAttribute("wrapping", ::fwAtoms::String::New("CLAMP"));

    helper.addAttribute("diffuse_texture", dtm);

    // Check that when converting a V5 Material object to a V4 Material object
    // We move from a single image member to a map of Texture objects
    {
        // Create an object and apply the patch
        matObjV4 = ::fwAtoms::Object::dynamicCast(matObjV5->clone());

        ::fwAtomsPatch::IPatch::NewVersionsType newVersions;
        newVersions[matObjV5] = matObjV4;

        ::fwStructuralPatch::fwData::Material::V5ToV4::sptr mat5ToMat4Patch;
        mat5ToMat4Patch = ::fwStructuralPatch::fwData::Material::V5ToV4::New();
        CPPUNIT_ASSERT_NO_THROW(mat5ToMat4Patch->apply(matObjV5, matObjV4, newVersions));

        CPPUNIT_ASSERT(matObjV4);

        // The image is empty at initialization, so we don't check it
        ::fwAtoms::String::sptr filtering = matObjV4->getAttribute< ::fwAtoms::String>("diffuse_texture_filtering");
        CPPUNIT_ASSERT(filtering);
        CPPUNIT_ASSERT(filtering->getString() == ::fwAtoms::String::New("NEAREST")->getString());
        ::fwAtoms::String::sptr wrapping = matObjV4->getAttribute< ::fwAtoms::String>("diffuse_texture_wrapping");
        CPPUNIT_ASSERT(wrapping);
        CPPUNIT_ASSERT(wrapping->getString() == ::fwAtoms::String::New("CLAMP")->getString());
    }
}

//------------------------------------------------------------------------------
} //namespace Material
} //namespace fwData
} //namespace ut
} //namespace fwStructuralPatch
