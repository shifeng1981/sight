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

#ifndef __FWRUNTIME_RUNTIMEEXCEPTION_HPP__
#define __FWRUNTIME_RUNTIMEEXCEPTION_HPP__

#include <exception>
#include <string>

#include <fwCore/Exception.hpp>

#include "fwRuntime/config.hpp"


namespace fwRuntime
{


/**
 * @brief   Defines the runtime exception class.
 * @struct  RuntimeException
 * @date    2004-2009
 *
 */
struct FWRUNTIME_CLASS_API RuntimeException : public ::fwCore::Exception
{

    /**
     * @brief       Copy constructor.
     *
     * @param[in]   exception   an exception instance to copy
     */
    FWRUNTIME_API RuntimeException(const RuntimeException& exception) noexcept;

    /**
     * @brief       Constructor.
     *
     * @param[in]   message a string containing the exception message
     */
    FWRUNTIME_API RuntimeException(const std::string& message) noexcept;

    /**
     * @brief   Destructor : does nothing.
     */
    FWRUNTIME_API virtual ~RuntimeException() noexcept;

};


} // namespace fwRuntime


#endif // __FWRUNTIME_RUNTIMEEXCEPTION_HPP__
