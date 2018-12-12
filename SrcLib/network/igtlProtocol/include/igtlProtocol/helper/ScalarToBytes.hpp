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

#ifndef __IGTLPROTOCOL_HELPER_SCALARTOBYTES_HPP__
#define __IGTLPROTOCOL_HELPER_SCALARTOBYTES_HPP__

#include "igtlProtocol/config.hpp"

#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/static_assert.hpp>

#include <algorithm>
#include <string>

namespace igtlProtocol
{

namespace helper
{

namespace detail
{
/**
 * @union ConvertData
 *
 * @tparam T T must be a scalar type
 * @brief data structure for scalar and his representation in bytes
 */
template<typename T>
union ConvertData
{
    T scalar;
    char bytes[sizeof(T)];
};
} // namespace detail

/**
 *
 * @tparam T T must be a scalar type
 * @brief utility class to convert bytes to scalar and scalar to bytes
 */
template<typename T>
class IGTLPROTOCOL_CLASS_API ScalarToBytes
{
public:

    typedef std::vector< char > RawDataType;

    /**
     * @brief method to convert a scalar to his representation in bytes
     *
     * @param[in] scalar
     * @return the representation in bytes for the given scalar
     */
    static RawDataType toBytes(T const scalar)
    {
        ::igtlProtocol::helper::detail::ConvertData<T> convertData;
        RawDataType bytes;

        BOOST_STATIC_ASSERT(::boost::is_arithmetic<T>::value);
        convertData.scalar = scalar;
        bytes.resize(sizeof (T));

        std::copy(convertData.bytes, convertData.bytes + sizeof(T), bytes.begin());

        return bytes;
    }

    /**
     * @brief method to convert bytes to a scalar
     *
     * @param[in] bytes
     * @return the scalar value from bytes
     */
    static T fromBytes(const char* const bytes)
    {
        ::igtlProtocol::helper::detail::ConvertData<T> convertData;
        std::copy(bytes, bytes + sizeof(T), convertData.bytes);

        return convertData.scalar;
    }
};

} // namespace helper

} // namespace igtlProtocol


#endif // __IGTLPROTOCOL_HELPER_SCALARTOBYTES_HPP__
