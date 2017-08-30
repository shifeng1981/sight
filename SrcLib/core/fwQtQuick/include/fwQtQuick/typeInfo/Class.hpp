/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWQTQUICK_TYPEINFO_CLASS_HPP__
#define __FWQTQUICK_TYPEINFO_CLASS_HPP__

#include "fwQtQuick/config.hpp"
#include "fwQtQuick/typeInfo/Symbol.hpp"

#include <QQmlListProperty>

namespace fwQtQuick
{

namespace TypeInfo
{

class Property;
class Method;

class FWQTQUICK_API Class : public Symbol
{
Q_OBJECT
Q_PROPERTY(bool abstract READ isAbstract CONSTANT);
Q_PROPERTY(QQmlListProperty<fwQtQuick::TypeInfo::Property> properties READ getPropertyList CONSTANT);
Q_PROPERTY(QQmlListProperty<fwQtQuick::TypeInfo::Method> methods READ getMethodList CONSTANT);

public:
    static Register::Type<Class> Register;
    QQmlListProperty<Property> getPropertyList();
    QQmlListProperty<Method> getMethodList();
    void setAbstract(bool);
    bool isAbstract();
    void addProperty(QMetaProperty);
    void addMethod(QMetaMethod);

private:

    Class();
    friend class Symbol;
    bool m_abstract = false;
    QList<Property*> m_properties;
    QList<Method*> m_methods;
};

} // namespace TypeInfo
} // namespace fwQtQuick

#endif //__FWQTQUICK_TYPEINFO_CLASS_HPP__
