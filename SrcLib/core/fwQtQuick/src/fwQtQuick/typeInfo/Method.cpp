/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwQtQuick/typeInfo/Method.hpp"

namespace fwQtQuick
{
namespace TypeInfo
{

//-----------------------------------------------------------------------------

::fwQtQuick::Register::Type<Method> Method::Register;

//-----------------------------------------------------------------------------

Method* Method::Create(QMetaMethod metaMethod)
{
    auto name       = QString(metaMethod.name());
    auto paramTypes = QString();
    auto sequence   = false;

    if (name == "deleteLater")
    {
        return nullptr;
    }

    auto returnType = QString(metaMethod.typeName()).remove("*").remove("quick::").replace("::", ".");

    auto paramTypeList = metaMethod.parameterTypes();
    auto separator     = QString(", ");

    for (auto paramType : paramTypeList)
    {
        paramTypes.append(paramType).append(separator);
    }

    if (paramTypeList.count() > 0)
    {
        paramTypes.remove(paramTypes.length() - separator.length(), paramTypes.length());
    }

    if (paramTypeList.count() == 0)
    {
        paramTypes = "void";
    }

    if (returnType.contains("List"))
    {
        sequence = true;
        if (auto bracketIndex = returnType.lastIndexOf("<"))
        {
            returnType.remove(0, bracketIndex + 1);
            returnType = returnType.remove("<").remove(">");
        }
        else if (returnType.contains("string"))
        {
            returnType = "string";
        }
    }

    if (returnType.startsWith("Q") && returnType.length() > 2)
    {
        QChar character = returnType.at(1);
        returnType = returnType.remove(0, 2);
        returnType.push_front(character.toLower());
    }

    auto method = new Method();

    method->m_returnType = returnType;
    method->m_paramTypes = paramTypes;
    method->m_name       = name;
    method->m_sequence   = sequence;

    return method;
}

//-----------------------------------------------------------------------------

QString Method::getReturnType()
{
    return this->m_returnType;
}

//-----------------------------------------------------------------------------

QString Method::getParamTypes()
{
    return this->m_paramTypes;
}

//-----------------------------------------------------------------------------

bool Method::isSequence()
{
    return this->m_sequence;
}

//-----------------------------------------------------------------------------

QString Method::getName()
{
    return this->m_name;
}

//-----------------------------------------------------------------------------

} // namespace TypeInfo
} // namespace fwQtQuick
