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

#ifndef __IGTLPROTOCOL_MESSAGEFACTORY_HPP__
#define __IGTLPROTOCOL_MESSAGEFACTORY_HPP__

#include "igtlProtocol/config.hpp"
#include "igtlProtocol/exception/Conversion.hpp"

#include <fwTools/Failed.hpp>

#include <boost/function.hpp>
#include <igtlMessageBase.h>

#include <map>

namespace igtlProtocol
{

class IGTLPROTOCOL_CLASS_API MessageFactory
{
public:
    typedef ::boost::function< ::igtl::MessageBase::Pointer () > CreatorType;
    typedef std::map<std::string, CreatorType>                   CreatorContainer;

    /**
     * @brief initFactory initialize the factory
     * @return the map of equivalence between a string device type igtl and a
     * method to create the appropiate message
     */
    static CreatorContainer initFactory();
    /**
     * @brief create create a message with the type specified in parameter
     * @param[in] type
     * @return a smart pointer to a igtl message
     */
    static IGTLPROTOCOL_API ::igtl::MessageBase::Pointer create(std::string const& type);;
private:

    MessageFactory();
    ~MessageFactory();

    /**
     * @struct MessageMaker
     * struct contain create function to create igtl message.
     * Hack to compile in msvc in other compiler this struct is useless
     * (see assignement of  pointer function to a overload function to boost::function)
     * I have no choice to tell msvc take this overload because msvc don't like taking address of a overload template function
     * @tparam T class derivate from igtl::MessageBase
     */
    template<typename T, int withParam> struct MessageMaker;

    template<typename T>
    struct MessageMaker<T, false>
    {
        /**
         * @brief create message from type specified in template
         * @return a igtl::MessageBase smart pointer contain the message
         */
        static ::igtl::MessageBase::Pointer createMessage()
        {
            typename T::Pointer msg;

            msg = T::New();
            return ::igtl::MessageBase::Pointer (msg);
        }


    };

    template<typename T>
    struct MessageMaker<T, true>
    {
        static ::igtl::MessageBase::Pointer createMessage (std::string const& bodyType)
        {
            typename T::Pointer msg;

            msg = T::New(bodyType);
            return ::igtl::MessageBase::Pointer (msg);
        }
    };


private:
    /// Map of equivalence between a device type and igtl message creator method
    static CreatorContainer s_creators;
};

}//namespace igtlProtocol

#endif // __IGTLPROTOCOL_MESSAGEFACTORY_HPP__
