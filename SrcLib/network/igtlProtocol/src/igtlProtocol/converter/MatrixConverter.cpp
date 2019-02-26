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

#include "igtlProtocol/converter/MatrixConverter.hpp"

#include "igtlProtocol/DataConverter.hpp"

#include <fwData/TransformationMatrix3D.hpp>

#include <igtlTransformMessage.h>

namespace igtlProtocol
{
namespace converter
{
const std::string MatrixConverter::s_IGTL_TYPE          = "TRANSFORM";
const std::string MatrixConverter::s_FWDATA_OBJECT_TYPE = ::fwData::TransformationMatrix3D::classname();

converterRegisterMacro(::igtlProtocol::converter::MatrixConverter);

MatrixConverter::MatrixConverter()
{
}

//-----------------------------------------------------------------------------

MatrixConverter::~MatrixConverter()
{
}

//-----------------------------------------------------------------------------

::igtl::MessageBase::Pointer MatrixConverter::fromFwDataObject(::fwData::Object::csptr src) const
{
    ::fwData::TransformationMatrix3D::csptr srcMatrix = ::fwData::TransformationMatrix3D::dynamicConstCast(src);
    ::igtl::TransformMessage::Pointer msg;
    ::igtl::Matrix4x4 dest;

    msg = ::igtl::TransformMessage::New();
    msg->GetMatrix(dest);
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            dest[i][j] = srcMatrix->getCoefficient(i, j);
        }
    }
    msg->SetMatrix(dest);
    return ::igtl::MessageBase::Pointer(msg.GetPointer());
}

//-----------------------------------------------------------------------------

::fwData::Object::sptr MatrixConverter::fromIgtlMessage(const ::igtl::MessageBase::Pointer src) const
{
    ::igtl::Matrix4x4 matrix;
    ::igtl::TransformMessage* msg                  = dynamic_cast< ::igtl::TransformMessage* >(src.GetPointer());
    ::igtl::TransformMessage::Pointer srcTransform = ::igtl::TransformMessage::Pointer(msg);
    ::fwData::TransformationMatrix3D::sptr dest    = ::fwData::TransformationMatrix3D::New();
    srcTransform->GetMatrix(matrix);
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            dest->setCoefficient(i, j, matrix[i][j]);
        }
    }

    return dest;
}

//-----------------------------------------------------------------------------

IConverter::sptr MatrixConverter::New()
{
    return std::make_shared< MatrixConverter >();
}

//-----------------------------------------------------------------------------

std::string const& MatrixConverter::getIgtlType() const
{
    return MatrixConverter::s_IGTL_TYPE;
}

//-----------------------------------------------------------------------------

std::string const& MatrixConverter::getFwDataObjectType() const
{
    return MatrixConverter::s_FWDATA_OBJECT_TYPE;
}

} // namespace converter
} // namespace igtlProtocol
