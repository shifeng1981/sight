/************************************************************************
 *
 * Copyright (C) 2009-2019 IRCAD France
 * Copyright (C) 2012-2019 IHU Strasbourg
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

#include "fwRuntime/dl/Library.hpp"

#include "fwRuntime/dl/Posix.hpp"
#include "fwRuntime/dl/Win32.hpp"

namespace fwRuntime
{

namespace dl
{

//------------------------------------------------------------------------------

#if defined(linux) || defined(__linux) || defined(__APPLE__)
typedef Posix LibraryImplType;
#else
typedef Win32 LibraryImplType;
#endif

//------------------------------------------------------------------------------

Library::Library(const ::boost::filesystem::path& modulePath) noexcept :
    m_implementor(new LibraryImplType(modulePath))
{
}

//------------------------------------------------------------------------------

} // namespace dl

} // namespace fwRuntime
