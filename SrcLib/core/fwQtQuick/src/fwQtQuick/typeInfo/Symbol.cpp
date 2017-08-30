/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwQtQuick/typeInfo/Symbol.hpp"

#include "fwQtQuick/typeInfo/Class.hpp"
#include "fwQtQuick/typeInfo/Enum.hpp"
#include "fwQtQuick/typeInfo/EnumItem.hpp"
#include "fwQtQuick/typeInfo/List.hpp"

#include <QMetaEnum>

namespace fwQtQuick
{
namespace TypeInfo
{

//-----------------------------------------------------------------------------

QString Symbol::Get::ClassName(QMetaObject metaObject)
{
    return QString(metaObject.className()).section("::", 2, 2);
}

//-----------------------------------------------------------------------------

QString Symbol::Get::ClassPrefix(QMetaObject metaObject)
{
    return QString(metaObject.className()).section("::", 1, 1);
}

//-----------------------------------------------------------------------------

QString Symbol::Get::EnumName(QMetaEnum metaEnum)
{
    return QString(metaEnum.scope()).section("::", 2, 2) + "." + metaEnum.name();
}

//-----------------------------------------------------------------------------

QString Symbol::Get::EnumPrefix(QMetaEnum metaEnum)
{
    return QString(metaEnum.scope()).section("::", 1, 1);
}

//-----------------------------------------------------------------------------

Register::Type<Symbol> Symbol::Register;

//-----------------------------------------------------------------------------

auto Symbol::GetEnums()->QStringList& {
    static QStringList Enums { ".TransformOrigin" };
    return Enums;
}

//-----------------------------------------------------------------------------

void Symbol::MakeEnum(QMetaEnum metaEnum)
{
    auto name = Get::EnumName(metaEnum);

    if (GetEnums().contains(name))
    {
        return;
    }

    auto symbol = new Enum();

    symbol->m_prefix = Get::EnumPrefix(metaEnum);
    symbol->m_name   = name;
    symbol->m_type   = "enum";
    symbol->m_color  = "#E6713E";

    for (auto i = 0; i < metaEnum.keyCount(); ++i)
    {
        symbol->add(new EnumItem(metaEnum.key(i), metaEnum.value(i)));
    }

    GetEnums().append(name);

    List::Add(symbol);
}

//-----------------------------------------------------------------------------

void Symbol::MakeClass(QMetaObject metaObject)
{
    auto symbol = new Class();

    symbol->m_prefix = Get::ClassPrefix(metaObject);
    symbol->m_name   = Get::ClassName(metaObject);
    symbol->m_type   = "class";
    symbol->m_color  = "#319CD3";

    for (auto i = 0; i < metaObject.propertyCount(); ++i)
    {
        symbol->addProperty(metaObject.property(i));
    }

    for (auto i = 0; i < metaObject.methodCount(); ++i)
    {
        auto method = metaObject.method(i);
        if (method.methodType() == QMetaMethod::MethodType::Slot && method.access() == QMetaMethod::Access::Public)
        {
            symbol->addMethod(metaObject.method(i));
        }
    }

    List::Add(symbol);
}

//-----------------------------------------------------------------------------

void Symbol::MakeAbstractClass(QMetaObject metaObject)
{
    auto symbol = new Class();

    symbol->m_prefix = Get::ClassPrefix(metaObject);
    symbol->m_name   = Get::ClassName(metaObject);
    symbol->m_type   = "abstract";
    symbol->m_color  = "#9DA5B4";

    symbol->setAbstract(true);

    for (auto i = 0; i < metaObject.propertyCount(); ++i)
    {
        symbol->addProperty(metaObject.property(i));
    }

    List::Add(symbol);
}

//-----------------------------------------------------------------------------

bool Symbol::matches(const QString& filter)
{
    QStringList words = filter.simplified().split(" ", QString::SkipEmptyParts);

    bool match = false;

    for (auto word : words)
    {
        match = this->m_name.contains(word, Qt::CaseInsensitive) ||
                this->m_type.contains(word, Qt::CaseInsensitive) ||
                this->m_prefix.contains(word, Qt::CaseInsensitive);

        if (!match)
        {
            return false;
        }
    }

    return true;
}

//-----------------------------------------------------------------------------

const bool Symbol::operator < (const Symbol& other)
{
    return this->m_name < other.m_name;
}

//-----------------------------------------------------------------------------

QString Symbol::getName()
{
    return this->m_name;
}

//-----------------------------------------------------------------------------

QString Symbol::getPrefix()
{
    return this->m_prefix;
}

//-----------------------------------------------------------------------------

QString Symbol::getType()
{
    return this->m_type;
}

//-----------------------------------------------------------------------------

QColor Symbol::getColor()
{
    return this->m_color;
}

//-----------------------------------------------------------------------------

} // namespace TypeInfo
} // namespace fwQtQuick
