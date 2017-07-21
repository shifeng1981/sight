/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwStructuralPatch/fwData/Material/V5ToV4.hpp"

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

V5ToV4::V5ToV4() :
    ::fwAtomsPatch::IStructuralPatch()
{
    m_originClassname = "::fwData::Material";
    m_targetClassname = "::fwData::Material";
    m_originVersion   = "5";
    m_targetVersion   = "4";
}

// ----------------------------------------------------------------------------

V5ToV4::~V5ToV4()
{
}

// ----------------------------------------------------------------------------

V5ToV4::V5ToV4( const V5ToV4& cpy ) :
    ::fwAtomsPatch::IStructuralPatch(cpy)
{
}

// ----------------------------------------------------------------------------

void V5ToV4::apply( const ::fwAtoms::Object::sptr& previous,
                    const ::fwAtoms::Object::sptr& current,
                    ::fwAtomsPatch::IPatch::NewVersionsType& newVersions)
{
    IStructuralPatch::apply(previous, current, newVersions);

    // Update object version
    this->updateVersion(current);

    ::fwAtomsPatch::helper::Object helper(current);

    ::fwAtoms::Map::sptr dtm = previous->getAttribute< ::fwAtoms::Map>("diffuse_texture");
    if(dtm->size() > 1)
    {
        // Get the first available texture and put it the material
        ::fwAtoms::Object::sptr dt = ::fwAtoms::Object::dynamicCast(dtm->begin()->second);

        ::fwAtoms::Object::sptr image     = previous->getAttribute< ::fwAtoms::Object>("diffuse_texture");
        ::fwAtoms::String::sptr filtering = previous->getAttribute< ::fwAtoms::String>("diffuse_texture_filtering");
        ::fwAtoms::String::sptr wrapping  = previous->getAttribute< ::fwAtoms::String>("diffuse_texture_wrapping");

        helper.replaceAttribute("diffuse_texture", image);
        helper.addAttribute("diffuse_texture_filtering", filtering);
        helper.addAttribute("diffuse_texture_wrapping", wrapping);
    }
    else
    {
        helper.replaceAttribute("diffuse_texture", ::fwAtoms::Object::New());
        helper.addAttribute("diffuse_texture_filtering", ::fwAtoms::String::New("NEAREST"));
        helper.addAttribute("diffuse_texture_wrapping", ::fwAtoms::String::New("CLAMP"));
    }
}

} // namespace Material

} // namespace fwData

} // namespace fwStructuralPatch
