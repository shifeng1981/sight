/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWQTQUICK_TYPEINFO_METHOD_HPP__
#define __FWQTQUICK_TYPEINFO_METHOD_HPP__

#include "fwQtQuick/config.hpp"
#include "fwQtQuick/utils/Register.hpp"

#include <QObject>

namespace fwQtQuick
{
namespace TypeInfo
{

class FWQTQUICK_API Method : public QObject
{
Q_OBJECT
Q_PROPERTY(QString returnType READ getReturnType CONSTANT);
Q_PROPERTY(QString paramTypes READ getParamTypes CONSTANT);
Q_PROPERTY(QString name READ getName CONSTANT);
Q_PROPERTY(bool sequence READ isSequence CONSTANT);

public:
    static Register::Type<Method> Register;

    static Method* Create(QMetaMethod);
    QString getReturnType();
    QString getParamTypes();
    QString getName();
    bool isSequence();

private:
    Method() = default;

    bool m_sequence = false;
    QString m_returnType;
    QString m_paramTypes;
    QString m_name;
};
} // namespace TypeInfo
} // namespace fwQtQuick

#endif //__FWQTQUICK_TYPEINFO_METHOD_HPP__
