/************************************************************************
 *
 * Copyright (C) 2009-2015 IRCAD France
 * Copyright (C) 2012-2015 IHU Strasbourg
 *
 * This file is part of Sight.
 *
 * Sight is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Sight. If not, see <https://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#ifndef __FWATOMSPATCH_CONDITIONS_ABSTRACT_HPP__
#define __FWATOMSPATCH_CONDITIONS_ABSTRACT_HPP__

#include <fwCore/base.hpp>

#include <fwAtoms/Base.hpp>

#include "fwAtomsPatch/config.hpp"

namespace fwAtomsPatch
{
namespace conditions
{

/**
 * @class Abstract
 * @brief Defines a condition in the patch.
 */
class FWATOMSPATCH_CLASS_API Abstract : public ::fwCore::BaseObject
{
public:
    fwCoreClassDefinitionsWithFactoryMacro((Abstract), (()), new Abstract );
    fwCoreAllowSharedFromThis();

    /// Default value getter
    FWATOMSPATCH_API virtual ::fwAtoms::Base::sptr getDefaultValue();

    /// Test in replacement value
    FWATOMSPATCH_API virtual bool test(::fwAtoms::Base::sptr old, ::fwAtoms::Base::sptr newValue);

    /// Test with one value (remove/add/rename)
    FWATOMSPATCH_API virtual bool test(::fwAtoms::Base::sptr old);

protected:
    Abstract()
    {
    }
    Abstract(::fwAtoms::Base::sptr defaultValue);

    ::fwAtoms::Base::sptr m_defaultValue;
    std::string m_message;

};

} // namespace conditions

} // namespace fwAtomsPatch

#endif /* __FWATOMSPATCH_CONDITIONS_ABSTRACT_HPP__ */
