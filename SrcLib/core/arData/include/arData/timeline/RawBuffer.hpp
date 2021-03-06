/************************************************************************
 *
 * Copyright (C) 2009-2016 IRCAD France
 * Copyright (C) 2012-2016 IHU Strasbourg
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

#ifndef __ARDATA_TIMELINE_RAWBUFFER_HPP__
#define __ARDATA_TIMELINE_RAWBUFFER_HPP__

#include <boost/function.hpp>

#include "arData/timeline/Buffer.hpp"
#include "arData/config.hpp"

namespace arData
{

namespace timeline
{

/**
 * @brief   This class defines a TimeLine object buffer.
 */
class ARDATA_CLASS_API RawBuffer : public arData::timeline::Buffer
{
public:

    /// Constructor
    ARDATA_API RawBuffer(::fwCore::HiResClock::HiResClockType timestamp = 0,
                         BufferDataType buffer = 0, size_t size = 0, DeleterType d = 0);

    /// Destructor
    ARDATA_API virtual ~RawBuffer();

    /// Set buffer values
    ARDATA_API void setBufferValues(BufferDataType values);

    /// Return buffer
    BufferDataType getBuffer() const
    {
        return m_buffer;
    }

    template<typename TYPE>
    TYPE* getBuffer() const
    {
        return reinterpret_cast<TYPE*>(m_buffer);
    }
};

} // namespace timeline

} // namespace arData

#endif /* __ARDATA_TIMELINE_RAWBUFFER_HPP__ */
