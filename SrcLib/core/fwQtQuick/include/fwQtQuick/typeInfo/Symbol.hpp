/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWQTQUICK_TYPEINFO_SYMBOL_HPP__
#define __FWQTQUICK_TYPEINFO_SYMBOL_HPP__

#include "fwQtQuick/config.hpp"
#include "fwQtQuick/utils/Register.hpp"

#include <QColor>
#include <QMetaObject>
#include <QObject>

namespace fwQtQuick
{
namespace TypeInfo
{

class FWQTQUICK_API Symbol : public QObject
{
Q_OBJECT
Q_PROPERTY(QColor color READ getColor CONSTANT);
Q_PROPERTY(QString name READ getName CONSTANT);
Q_PROPERTY(QString type READ getType CONSTANT);
Q_PROPERTY(QString prefix READ getPrefix CONSTANT);

public:
    static Register::Type<Symbol> Register;

    static void MakeEnum(QMetaEnum);
    static void MakeClass(QMetaObject);
    static void MakeAbstractClass(QMetaObject);
    const bool operator < (const Symbol&);
    QColor getColor();
    QString getType();
    QString getName();
    QString getPrefix();
    bool matches(const QString&);

private:
    struct Get
    {
        static QString ClassName(QMetaObject);
        static QString ClassPrefix(QMetaObject);
        static QString EnumName(QMetaEnum);
        static QString EnumPrefix(QMetaEnum);
    };
    static QStringList& GetEnums();
    QColor m_color;
    QString m_type;
    QString m_name;
    QString m_prefix;
};

} // namespace TypeInfo
} // namespace fwQtQuick

#endif //__FWQTQUICK_TYPEINFO_SYMBOL_HPP__
