/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWQTQUICK_TYPEINFO_ENUM_HPP__
#define __FWQTQUICK_TYPEINFO_ENUM_HPP__

#include "fwQtQuick/config.hpp"
#include "fwQtQuick/typeInfo/Symbol.hpp"

#include <QQmlListProperty>

namespace fwQtQuick
{
namespace TypeInfo
{

class EnumItem;

class FWQTQUICK_API Enum : public Symbol
{
Q_OBJECT
Q_PROPERTY(QQmlListProperty<fwQtQuick::TypeInfo::EnumItem> enumerations READ getEnumItemList CONSTANT);

public:
    static Register::Type<Enum> Register;
    void add(EnumItem*);
    QQmlListProperty<EnumItem> getEnumItemList();

private:
    friend class Symbol;
    Enum();
    QList<EnumItem*> m_enumItems;
};
} // namespace TypeInfo
} // namespace fwQtQuick

#endif //__FWQTQUICK_TYPEINFO_ENUM_HPP__
