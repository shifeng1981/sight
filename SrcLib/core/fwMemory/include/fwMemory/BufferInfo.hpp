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

#ifndef __FWMEMORY_BUFFERINFO_HPP__
#define __FWMEMORY_BUFFERINFO_HPP__

#include "fwMemory/BufferAllocationPolicy.hpp"
#include "fwMemory/FileFormat.hpp"
#include "fwMemory/FileHolder.hpp"
#include "fwMemory/config.hpp"
#include "fwMemory/stream/in/IFactory.hpp"

#include <fwCore/LogicStamp.hpp>
#include <fwCore/macros.hpp>

#include <boost/filesystem/path.hpp>

namespace fwMemory
{

struct FWMEMORY_CLASS_API BufferInfo
{

    typedef size_t SizeType;
    typedef WPTR ( void ) CounterType;

    FWMEMORY_API BufferInfo();

    FWMEMORY_API void clear();
    //------------------------------------------------------------------------------

    long lockCount() const
    {
        return lockCounter.use_count();
    }

    SizeType size;
    /// true if 'buffer' is loaded
    bool loaded;
    bool userStreamFactory;

    FileHolder fsFile;
    FileFormatType fileFormat;

    CounterType lockCounter;

    ::fwCore::LogicStamp lastAccess;
    ::fwMemory::BufferAllocationPolicy::sptr bufferPolicy;

    SPTR( ::fwMemory::stream::in::IFactory ) istreamFactory;

};

} // namespace fwMemory

#endif // __FWMEMORY_BUFFERINFO_HPP__

