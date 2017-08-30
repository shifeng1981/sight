/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWQTQUICK_TYPEINFO_ENUMITEM_HPP__
#define __FWQTQUICK_TYPEINFO_ENUMITEM_HPP__

#include "fwQtQuick/config.hpp"
#include "fwQtQuick/utils/Register.hpp"

#include <QObject>

namespace fwQtQuick
{
namespace TypeInfo
{

class FWQTQUICK_API EnumItem : public QObject
{
Q_OBJECT
Q_PROPERTY(QString name READ getName CONSTANT);
Q_PROPERTY(int value READ getValue CONSTANT);

public:
    static Register::Type<EnumItem> Register;
    EnumItem() = delete;
    EnumItem(const QString&, const int);
    QString getName();
    int getValue();

private:
    QString m_name;
    int m_value;
};
} // namespace TypeInfo
} // namespace fwQtQuick

#endif //__FWQTQUICK_TYPEINFO_ENUMITEM_HPP__
