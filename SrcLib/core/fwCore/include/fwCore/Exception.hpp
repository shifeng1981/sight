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


#ifndef __FWCORE_EXCEPTION_HPP__
#define __FWCORE_EXCEPTION_HPP__


#include <stdexcept>
#include <string>

#include <boost/exception/exception.hpp>

#include "fwCore/config.hpp"

namespace fwCore
{


class FWCORE_CLASS_API Exception : public std::runtime_error
{

public:
    FWCORE_API Exception ( const std::string &err );

};

} //namespace fwCore

#endif //__FWCORE_EXCEPTION_HPP__
