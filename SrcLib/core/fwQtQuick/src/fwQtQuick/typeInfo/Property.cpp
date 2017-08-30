/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwQtQuick/typeInfo/Property.hpp"

namespace fwQtQuick
{
namespace TypeInfo
{

//-----------------------------------------------------------------------------

::fwQtQuick::Register::Type<Property> Property::Register;

//-----------------------------------------------------------------------------

Property* Property::Create(QMetaProperty metaProperty)
{
    auto name = QString(metaProperty.name());

    if (name == "objectName")
    {
        return nullptr;
    }

    auto sequence = false;
    auto type     = QString(metaProperty.typeName()).remove("*").remove("quick::").replace("::", ".");

    if (type.contains("List"))
    {
        sequence = true;
        if (auto bracketIndex = type.lastIndexOf("<"))
        {
            type.remove(0, bracketIndex + 1);
            type = type.remove("<").remove(">");
        }
        else if (type.contains("string"))
        {
            type = "string";
        }
    }

    if (type.startsWith("Q") && type.length() > 2)
    {
        QChar character = type.at(1);
        type = type.remove(0, 2);
        type.push_front(character.toLower());
    }

    auto property = new Property();

    property->m_type     = type;
    property->m_name     = name;
    property->m_sequence = sequence;
    property->m_readable = metaProperty.isReadable();
    property->m_writable = metaProperty.isWritable();

    return property;
}

//-----------------------------------------------------------------------------

QString Property::getType()
{
    return this->m_type;
}

//-----------------------------------------------------------------------------

QString Property::getName()
{
    return this->m_name;
}

//-----------------------------------------------------------------------------

bool Property::isSequence()
{
    return this->m_sequence;
}

//-----------------------------------------------------------------------------

bool Property::isWritable()
{
    return this->m_writable;
}

//-----------------------------------------------------------------------------

bool Property::isReadable()
{
    return this->m_readable;
}

//-----------------------------------------------------------------------------

} // namespace TypeInfo
} // namespace fwQtQuick
