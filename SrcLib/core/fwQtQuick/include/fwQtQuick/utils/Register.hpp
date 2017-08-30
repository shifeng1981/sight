/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWQTQUICK_UTILS_REGISTER_HPP__
#define __FWQTQUICK_UTILS_REGISTER_HPP__

#include "fwQtQuick/config.hpp"

#include <QList>
#include <QMetaObject>
#include <QMetaProperty>
#include <QString>
#include <QtQml>

#include <functional>

namespace fwQtQuick
{
namespace Register
{
namespace MakeSymbol
{
FWQTQUICK_API void Enum(QMetaEnum);
FWQTQUICK_API void Class(QMetaObject);
FWQTQUICK_API void AbstractClass(QMetaObject);
} // namespace MakeSymbol

FWQTQUICK_API void Init();

#ifdef _MSC_VER
struct Queue
{
    static auto GetList()->QList<std::function<void(void)> >&;
};
#endif

template <class T>
struct Controller
{
    Controller()
    {
        auto initializer = []()
                           {
                               qmlRegisterType<T>();
                               T::Create();
                           };
#ifdef _MSC_VER
        Queue::GetList().append(initializer);
#else
        initializer();
#endif
    }
};

template <class T>
struct Type
{
    Type()
    {
        auto initializer = []()
                           {
                               qmlRegisterType<T>();
                           };
#ifdef _MSC_VER
        Queue::GetList().append(initializer);
#else
        initializer();
#endif
    }
};

template <class T>
struct AbstractClass
{
    AbstractClass()
    {
        auto initializer = []()
                           {
                               QMetaObject metaObject = T::staticMetaObject;
                               auto name              = QString(metaObject.className());
                               auto prefix            = name.section("::", 1, 1);
                               auto className         = name.section("::", 2, 2);

                               qmlRegisterUncreatableType<T>(prefix.toStdString().c_str(), 1, 0,
                                                             className.toStdString().c_str(),
                                                             "abstract class \'" + className +
                                                             "\' can not be instantiated.");

                               for (auto i = 0; i < metaObject.enumeratorCount(); ++i)
                               {
                                   MakeSymbol::Enum(metaObject.enumerator(i));
                               }

                               MakeSymbol::AbstractClass(metaObject);
                           };
#ifdef _MSC_VER
        Queue::GetList().append(initializer);
#else
        initializer();
#endif
    }
};

template <class T>
struct UncreatableClass
{
    UncreatableClass()
    {
        auto initializer = []()
                           {
                               QMetaObject metaObject = T::staticMetaObject;
                               auto name              = QString(metaObject.className());
                               auto prefix            = name.section("::", 1, 1);
                               auto className         = name.section("::", 2, 2);

                               qmlRegisterUncreatableType<T>(prefix.toStdString().c_str(), 1, 0,
                                                             className.toStdString().c_str(),
                                                             "\'" + className + "\' can not be instantiated.");

                               for (auto i = 0; i < metaObject.enumeratorCount(); ++i)
                               {
                                   MakeSymbol::Enum(metaObject.enumerator(i));
                               }

                               MakeSymbol::Class(metaObject);
                           };
#ifdef _MSC_VER
        Queue::GetList().append(initializer);
#else
        initializer();
#endif
    }
};

template <class T>
struct Class
{
    Class()
    {
        auto initializer = []()
                           {
                               QMetaObject metaObject = T::staticMetaObject;
                               auto name              = QString(metaObject.className());
                               auto prefix            = name.section("::", 1, 1);
                               auto className         = name.section("::", 2, 2);

                               qmlRegisterType<T>(prefix.toStdString().c_str(), 1, 0, className.toStdString().c_str());

                               for (auto i = 0; i < metaObject.enumeratorCount(); ++i)
                               {
                                   MakeSymbol::Enum(metaObject.enumerator(i));
                               }

                               MakeSymbol::Class(metaObject);
                           };
#ifdef _MSC_VER
        Queue::GetList().append(initializer);
#else
        initializer();
#endif
    }
};

} // namespace Register
} //namespace fwQtQuick

#endif //__FWQTQUICK_UTILS_REGISTER_HPP__
