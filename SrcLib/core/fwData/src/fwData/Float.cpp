/************************************************************************
 *
 * Copyright (C) 2009-2017 IRCAD France
 * Copyright (C) 2012-2017 IHU Strasbourg
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

#include "fwData/Float.hpp"

#include "fwData/Exception.hpp"
#include "fwData/GenericField.hpp"
#include "fwData/registry/macros.hpp"

fwDataRegisterMacro( ::fwData::Float );

namespace fwData
{

//------------------------------------------------------------------------------

Float::Float( ) noexcept
{
}

//------------------------------------------------------------------------------

Float::Float( ::fwData::Object::Key key ) noexcept
{
}

//------------------------------------------------------------------------------

Float::~Float() noexcept
{
}

//------------------------------------------------------------------------------
void Float::shallowCopy(const Object::csptr& _source )
{
    Float::csptr other = Float::dynamicConstCast(_source);
    FW_RAISE_EXCEPTION_IF( ::fwData::Exception(
                               "Unable to copy" + (_source ? _source->getClassname() : std::string("<NULL>"))
                               + " to " + this->getClassname()), !bool(other) );
    this->fieldShallowCopy( _source );
    m_value = other->m_value;
}

//------------------------------------------------------------------------------

void Float::cachedDeepCopy(const Object::csptr& _source, DeepCopyCacheType& cache)
{
    Float::csptr other = Float::dynamicConstCast(_source);
    FW_RAISE_EXCEPTION_IF( ::fwData::Exception(
                               "Unable to copy" + (_source ? _source->getClassname() : std::string("<NULL>"))
                               + " to " + this->getClassname()), !bool(other) );
    this->fieldDeepCopy( _source, cache );
    m_value = other->m_value;
}

} // namespace fwData
