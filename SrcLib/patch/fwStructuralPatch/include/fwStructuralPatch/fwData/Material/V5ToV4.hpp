/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWSTRUCTURALPATCH_FWDATA_MATERIAL_V5TOV4_HPP__
#define __FWSTRUCTURALPATCH_FWDATA_MATERIAL_V5TOV4_HPP__

#include "fwStructuralPatch/config.hpp"

#include <fwAtomsPatch/IStructuralPatch.hpp>

namespace fwStructuralPatch
{

namespace fwData
{

namespace Material
{

/// Structural patch to convert a fwData::Material from version '5' to version '4'.
class FWSTRUCTURALPATCH_CLASS_API V5ToV4 : public ::fwAtomsPatch::IStructuralPatch
{
public:
    fwCoreClassDefinitionsWithFactoryMacro(
        (V5ToV4)(::fwStructuralPatch::fwData::Material::V5ToV4), (()), new V5ToV4);

    /// Constructor
    FWSTRUCTURALPATCH_API V5ToV4();

    /// Destructor
    FWSTRUCTURALPATCH_API ~V5ToV4();

    /// Copy constructor
    FWSTRUCTURALPATCH_API V5ToV4( const V5ToV4& cpy );

    /**
     * @brief Applies patch
     *
     * Added new map of additional array.
     */
    FWSTRUCTURALPATCH_API virtual void apply(
        const ::fwAtoms::Object::sptr& previous,
        const ::fwAtoms::Object::sptr& current,
        ::fwAtomsPatch::IPatch::NewVersionsType& newVersions);

};

} // namespace Material

} // namespace fwData

} // namespace fwStructuralPatch

#endif /* __FWSTRUCTURALPATCH_FWDATA_MATERIAL_V5TOV4_HPP__ */
