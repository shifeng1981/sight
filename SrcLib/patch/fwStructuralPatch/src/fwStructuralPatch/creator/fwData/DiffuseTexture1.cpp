/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwStructuralPatch/creator/fwData/DiffuseTexture1.hpp"

#include <fwAtoms/Object.hpp>
#include <fwAtoms/String.hpp>

#include <fwAtomsPatch/helper/Object.hpp>

namespace fwStructuralPatch
{
namespace creator
{
namespace fwData
{

// ----------------------------------------------------------------------------

DiffuseTexture1::DiffuseTexture1()
{
    m_classname = "::fwData::DiffuseTexture";
    m_version   = "1";
}

// ----------------------------------------------------------------------------

DiffuseTexture1::~DiffuseTexture1()
{
}

// ----------------------------------------------------------------------------

DiffuseTexture1::DiffuseTexture1( const DiffuseTexture1& cpy ) :
    ::fwAtomsPatch::IStructuralCreator(cpy)
{
}

// ----------------------------------------------------------------------------

::fwAtoms::Object::sptr DiffuseTexture1::create()
{
    ::fwAtoms::Object::sptr pl = this->createObjBase();
    ::fwAtomsPatch::helper::Object helper(pl);

    helper.addAttribute("image", ::fwAtoms::Object::New());
    helper.addAttribute("filtering", ::fwAtoms::String::New("NEAREST"));
    helper.addAttribute("wrapping", ::fwAtoms::String::New("CLAMP"));
    helper.addAttribute("blending", ::fwAtoms::String::New("NONE"));

    return pl;
}

// ----------------------------------------------------------------------------

} // namespace fwData
} // namespace creator
} // namespace fwStructuralPatch
