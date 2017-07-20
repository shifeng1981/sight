/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWSTRUCTURALPATCH_CREATOR_FWDATA_DIFFUSETEXTURE1_HPP__
#define __FWSTRUCTURALPATCH_CREATOR_FWDATA_DIFFUSETEXTURE1_HPP__

#include "fwStructuralPatch/config.hpp"

#include <fwAtomsPatch/IStructuralCreator.hpp>

namespace fwStructuralPatch
{
namespace creator
{
namespace fwData
{

/// Structural creator for ::fwData::DiffuseTexture version '1'.
class FWSTRUCTURALPATCH_CLASS_API DiffuseTexture1 : public ::fwAtomsPatch::IStructuralCreator
{
public:
    fwCoreClassDefinitionsWithFactoryMacro( (DiffuseTexture1)(::fwAtomsPatch::IStructuralCreator), (()),
                                            new DiffuseTexture1);

    /// Constructor
    FWSTRUCTURALPATCH_API DiffuseTexture1();

    /// Destructor
    FWSTRUCTURALPATCH_API ~DiffuseTexture1();

    /// Copy constructor
    FWSTRUCTURALPATCH_API DiffuseTexture1( const DiffuseTexture1& cpy );

    /// Create the specified object (sets 'points' attribute).
    FWSTRUCTURALPATCH_API virtual ::fwAtoms::Object::sptr create();

};

} // namespace fwData
} // namespace creator
} // namespace fwStructuralPatch

#endif // __FWSTRUCTURALPATCH_CREATOR_FWDATA_DIFFUSETEXTURE1_HPP__
