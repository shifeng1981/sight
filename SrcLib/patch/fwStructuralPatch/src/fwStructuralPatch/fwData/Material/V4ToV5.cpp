/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2015-2016.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwStructuralPatch/fwData/Material/V4ToV5.hpp"

#include <fwAtoms/Boolean.hpp>
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

V4ToV5::V4ToV5() : ::fwAtomsPatch::IStructuralPatch()
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

V4ToV5::V4ToV5( const V4ToV5 &cpy ) : ::fwAtomsPatch::IStructuralPatch(cpy)
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
    ::fwAtoms::Object::sptr image = previous->getAttribute< ::fwAtoms::Object>("diffuse_texture");
    ::fwAtoms::String::sptr diffuseTextureFiltering = previous->getAttribute< ::fwAtoms::String>("diffuse_texture_filtering");
    ::fwAtoms::String::sptr diffuseTextureWrapping = previous->getAttribute< ::fwAtoms::String>("diffuse_texture_wrapping");

    //std::map<std::string, ::fwData::DiffuseTexture::sptr> m_diffuseTexture;

    ::fwAtomsPatch::helper::Object helper(current);

    helper.removeAttribute("diffuse_texture_filtering");
    helper.removeAttribute("diffuse_texture_wrapping");

    // Switch diffuse and ambient
    ::fwAtoms::Object::sptr diffuseTexture = current->getAttribute< ::fwAtoms::Object >("diffuse_texture");

    // Replace diffuse by previous ambient
    //helper.replaceAttribute("image", image);
    //helper.replaceAttribute("filtering", diffuseTextureFiltering);
    //helper.replaceAttribute("wrapping", diffuseTextureWrapping);
}

} // namespace Material

} // namespace fwData

} // namespace fwStructuralPatch
