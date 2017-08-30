/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwQtQuick/typeInfo/EnumItem.hpp"

namespace fwQtQuick
{
namespace TypeInfo
{

//-----------------------------------------------------------------------------

::fwQtQuick::Register::Type<EnumItem> EnumItem::Register;

//-----------------------------------------------------------------------------

EnumItem::EnumItem(const QString& name, const int value) :
    m_name(name),
    m_value(value)
{
    //----
}

//-----------------------------------------------------------------------------

QString EnumItem::getName()
{
    return this->m_name;
}

//-----------------------------------------------------------------------------

int EnumItem::getValue()
{
    return this->m_value;
}

//-----------------------------------------------------------------------------

} // namespace TypeInfo
} // namespace fwQtQuick
