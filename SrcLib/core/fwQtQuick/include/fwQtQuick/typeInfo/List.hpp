/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWQTQUICK_TYPEINFO_LIST_HPP__
#define __FWQTQUICK_TYPEINFO_LIST_HPP__

#include "fwQtQuick/config.hpp"
#include "fwQtQuick/utils/Register.hpp"

#include <QAbstractListModel>

namespace fwQtQuick
{
namespace TypeInfo
{

class Symbol;

class FWQTQUICK_API List : public QAbstractListModel
{
Q_OBJECT
Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibleChanged);
Q_PROPERTY(QString filter READ getFilter WRITE setFilter NOTIFY filterChanged);
Q_PROPERTY(int totalCount READ getTotalCount CONSTANT);
Q_PROPERTY(int count READ getCount NOTIFY countChanged);

public:
    enum Roles
    {
        SymbolRole = Qt::UserRole + 1
    };

    static Register::Type<List> Register;
    static List* GetInstance();
    void init();
    void setVisible(bool);
    bool isVisible();
    void setFilter(const QString&);
    QString getFilter();
    int getCount();
    int getTotalCount();
    int rowCount(const QModelIndex& = QModelIndex()) const;
    QVariant data(const QModelIndex&, int) const;
    QHash<int, QByteArray> roleNames() const;

Q_SIGNALS:
    void visibleChanged();
    void filterChanged();
    void countChanged();

private:
    friend class Symbol;
    bool m_visible = false;
    QString m_filter;
    QList<Symbol*> m_symbols;
    QList<Symbol*> m_allSymbols;
    static void Add(Symbol*);
};
} // namespace TypeInfo
} // namespace fwQtQuick

#endif //__FWQTQUICK_TYPEINFO_LIST_HPP__
