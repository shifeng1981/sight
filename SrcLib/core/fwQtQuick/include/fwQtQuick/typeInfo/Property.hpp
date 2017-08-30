/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWQTQUICK_TYPEINFO_PROPERTY_HPP__
#define __FWQTQUICK_TYPEINFO_PROPERTY_HPP__

#include "fwQtQuick/config.hpp"
#include "fwQtQuick/utils/Register.hpp"

#include <QObject>

namespace fwQtQuick
{
namespace TypeInfo
{

class FWQTQUICK_API Property : public QObject
{
Q_OBJECT
Q_PROPERTY(bool sequence READ isSequence CONSTANT);
Q_PROPERTY(bool writable READ isWritable CONSTANT);
Q_PROPERTY(bool readable READ isReadable CONSTANT);
Q_PROPERTY(QString type READ getType CONSTANT);
Q_PROPERTY(QString name READ getName CONSTANT);

public:
    static Register::Type<Property> Register;

    static Property* Create(QMetaProperty);
    bool isSequence();
    bool isWritable();
    bool isReadable();
    QString getType();
    QString getName();

private:
    Property() = default;
    bool m_writable = false;
    bool m_readable = false;
    bool m_sequence = false;
    QString m_type;
    QString m_name;
};
} // namespace TypeInfo
} // namespace fwQtQuick

#endif //__FWQTQUICK_TYPEINFO_PROPERTY_HPP__
