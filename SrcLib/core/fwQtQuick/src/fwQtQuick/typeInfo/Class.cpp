/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwQtQuick/typeInfo/Class.hpp"

#include "fwQtQuick/typeInfo/Method.hpp"
#include "fwQtQuick/typeInfo/Property.hpp"

namespace fwQtQuick
{
namespace TypeInfo
{

//-----------------------------------------------------------------------------

Class::Class()
{
}

//-----------------------------------------------------------------------------

void Class::setAbstract(bool abstract)
{
    this->m_abstract = abstract;
}

//-----------------------------------------------------------------------------

bool Class::isAbstract()
{
    return this->m_abstract;
}

//-----------------------------------------------------------------------------

QQmlListProperty<Property> Class::getPropertyList()
{
    return QQmlListProperty<Property>(this, this->m_properties);
}

//-----------------------------------------------------------------------------

QQmlListProperty<Method> Class::getMethodList()
{
    return QQmlListProperty<Method>(this, this->m_methods);
}

//-----------------------------------------------------------------------------

void Class::addProperty(QMetaProperty metaProperty)
{
    auto property = Property::Create(metaProperty);

    if (property)
    {
        this->m_properties.append(property);
    }
}

//-----------------------------------------------------------------------------

void Class::addMethod(QMetaMethod metaMethod)
{
    auto method = Method::Create(metaMethod);

    if (method)
    {
        this->m_methods.append(method);
    }
}

//-----------------------------------------------------------------------------

} // namespace TypeInfo
} // namespace fwQtQuick
