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

#ifndef __IGTLPROTOCOL_CONVERTER_LINECONVERTER_HPP__
#define __IGTLPROTOCOL_CONVERTER_LINECONVERTER_HPP__

#include "igtlProtocol/converter/IConverter.hpp"
#include "igtlProtocol/exception/Conversion.hpp"

namespace igtlProtocol
{
namespace converter
{
/**
 *
 * @brief class to manage conversion between fwData::Line and igtl::PositionMessage
 */
class IGTLPROTOCOL_CLASS_API LineConverter :
    public IConverter
{
public:
    /// Constructor
    IGTLPROTOCOL_API LineConverter();

    /// Destructor
    IGTLPROTOCOL_API ~LineConverter();

    /**
     * @brief convert a igtl::PositionMessage to a ::fwData::Line
     *
     * @return an fwData::Image converted from an igtl::ImageMessage
     */
    IGTLPROTOCOL_API ::fwData::Object::sptr fromIgtlMessage (::igtl::MessageBase::Pointer const src) const;

    /**
     * @brief convert a ::fwData::Line to an igtl::PositionMessage
     *
     * @return an  igtl::ImageMessage converted from an fwData::Image
     */
    IGTLPROTOCOL_API ::igtl::MessageBase::Pointer fromFwDataObject (::fwData::Object::csptr src) const;

    /**
     * @brief create a new LineConverter smart pointer
     *
     * @return a smart pointer to an ImageConverter
     */
    IGTLPROTOCOL_API static IConverter::sptr New();

    /**
     * @brief get the igtlType supported for conversion
     *
     * @return the igtlType supported for conversion
     */
    IGTLPROTOCOL_API std::string const& getIgtlType() const;

    /**
     * @brief get the fwData object type supported for conversion
     *
     * @return the fwData Object type supported for conversion
     */
    IGTLPROTOCOL_API std::string const& getFwDataObjectType() const;

private:

    /// igtl type supported for conversion
    static std::string const s_IGTL_TYPE;

    /// fwData type supported for conversion
    static std::string const s_FWDATA_OBJECT_TYPE;
};
} // namespace converter
} // namespace igtlProtocol

#endif // __IGTLPROTOCOL_CONVERTER_LINECONVERTER_HPP__
