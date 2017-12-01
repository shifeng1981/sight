/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwStructuralPatch/fwData/Material/V4ToV5.hpp"

#include <fwAtoms/Boolean.hpp>
#include <fwAtoms/Map.hpp>
#include <fwAtoms/Numeric.hpp>
#include <fwAtoms/Numeric.hxx>
#include <fwAtoms/Object.hpp>
#include <fwAtoms/Object.hxx>
#include <fwAtoms/Sequence.hpp>
#include <fwAtoms/String.hpp>

#include <fwAtomsPatch/StructuralCreatorDB.hpp>

namespace fwStructuralPatch
{

namespace fwData
{

namespace Material
{

V4ToV5::V4ToV5() :
    ::fwAtomsPatch::IStructuralPatch()
{
    m_originClassname = "::fwData::Material";
    m_targetClassname = "::fwData::Material";
    m_originVersion   = "4";
    m_targetVersion   = "5";
}

// ----------------------------------------------------------------------------

V4ToV5::~V4ToV5()
{
}

// ----------------------------------------------------------------------------

V4ToV5::V4ToV5( const V4ToV5& cpy ) :
    ::fwAtomsPatch::IStructuralPatch(cpy)
{
}

// ----------------------------------------------------------------------------

void V4ToV5::apply( const ::fwAtoms::Object::sptr& previous,
                    const ::fwAtoms::Object::sptr& current,
                    ::fwAtomsPatch::IPatch::NewVersionsType& newVersions)
{
    IStructuralPatch::apply(previous, current, newVersions);

    // Update object version
    this->updateVersion(current);

    // Get Previous attributes
    ::fwAtoms::Object::sptr image     = previous->getAttribute< ::fwAtoms::Object>("diffuse_texture");
    ::fwAtoms::String::sptr filtering = previous->getAttribute< ::fwAtoms::String>("diffuse_texture_filtering");
    ::fwAtoms::String::sptr wrapping  = previous->getAttribute< ::fwAtoms::String>("diffuse_texture_wrapping");

    ::fwAtomsPatch::helper::Object helper(current);

    helper.removeAttribute("diffuse_texture_filtering");
    helper.removeAttribute("diffuse_texture_wrapping");

    ::fwAtomsPatch::StructuralCreatorDB::sptr creators = ::fwAtomsPatch::StructuralCreatorDB::getDefault();
    ::fwAtoms::Object::sptr dt1                        = creators->create( "::fwData::Texture", "1");
    ::fwAtomsPatch::helper::Object helperDT1(dt1);

    helperDT1.replaceAttribute("image", image);
    helperDT1.replaceAttribute("filtering", filtering);
    helperDT1.replaceAttribute("wrapping", wrapping);

    ::fwAtoms::Map::sptr dmap = ::fwAtoms::Map::New();
    dmap->insert("diffuse", dt1);

    // Switch diffuse and ambient
    helper.replaceAttribute("diffuse_texture", dmap);
}

} // namespace Material

} // namespace fwData

} // namespace fwStructuralPatch
