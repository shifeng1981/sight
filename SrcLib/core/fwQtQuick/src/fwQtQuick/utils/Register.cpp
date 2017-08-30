/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwQtQuick/utils/Register.hpp"

#include "fwQtQuick/typeInfo/List.hpp"
#include "fwQtQuick/typeInfo/Symbol.hpp"

namespace fwQtQuick
{
namespace Register
{
namespace MakeSymbol
{
//------------------------------------------------------------------------------

void Enum(QMetaEnum metaEnum)
{
    ::fwQtQuick::TypeInfo::Symbol::MakeEnum(metaEnum);
} //namespace MakeSymbol

//------------------------------------------------------------------------------

void Class(QMetaObject metaObject)
{
    ::fwQtQuick::TypeInfo::Symbol::MakeClass(metaObject);
}

//------------------------------------------------------------------------------

void AbstractClass(QMetaObject metaObject)
{
    ::fwQtQuick::TypeInfo::Symbol::MakeAbstractClass(metaObject);
}
} // namespace MakeSymbol

//------------------------------------------------------------------------------

void Init()
{
#ifdef _MSC_VER
    for (auto func : Queue::GetList())
    {
        func();
    }
#endif
    ::fwQtQuick::TypeInfo::List::GetInstance()->init();
}

#ifdef _MSC_VER
auto Queue::GetList()->QList<std::function<void(void)> >&
{
    static QList<std::function<void(void)> > List;
    return List;
}
#endif

//------------------------------------------------------------------------------

} //namespace Register
} //namespace fwQtQuick
