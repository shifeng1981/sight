/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwQtQuick/typeInfo/Enum.hpp"

#include "fwQtQuick/typeInfo/EnumItem.hpp"

namespace fwQtQuick
{
namespace TypeInfo
{

//-----------------------------------------------------------------------------

::fwQtQuick::Register::Type<Enum> Enum::Register;

//-----------------------------------------------------------------------------

Enum::Enum()
{
    //----
}

//-----------------------------------------------------------------------------

void Enum::add(EnumItem* item)
{
    this->m_enumItems.append(item);
}

//-----------------------------------------------------------------------------

QQmlListProperty<EnumItem> Enum::getEnumItemList()
{
    return QQmlListProperty<EnumItem>(this, this->m_enumItems);
}

//-----------------------------------------------------------------------------

} // namespace TypeInfo
} // namespace fwQtQuick
