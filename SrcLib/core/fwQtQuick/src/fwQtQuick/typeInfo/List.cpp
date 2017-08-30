/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwQtQuick/typeInfo/List.hpp"

#include "fwQtQuick/typeInfo/Symbol.hpp"

namespace fwQtQuick
{
namespace TypeInfo
{

//-----------------------------------------------------------------------------

::fwQtQuick::Register::Type<List> List::Register;

//-----------------------------------------------------------------------------

List* List::GetInstance()
{
    static List* instance = new List();
    return instance;
}

//-----------------------------------------------------------------------------

void List::setVisible(bool visible)
{
    if (this->m_visible != visible)
    {
        this->m_visible = visible;
        Q_EMIT this->visibleChanged();
    }
}

//-----------------------------------------------------------------------------

bool List::isVisible()
{
    return this->m_visible;
}

//-----------------------------------------------------------------------------

void List::setFilter(const QString& filter)
{
    this->m_filter = filter;
    Q_EMIT this->filterChanged();

    this->beginResetModel();
    this->m_symbols.clear();

    for (auto symbol : this->m_allSymbols)
    {
        if (symbol->matches(filter))
        {
            this->m_symbols.append(symbol);
        }
    }

    this->endResetModel();

    Q_EMIT this->countChanged();
}

//-----------------------------------------------------------------------------

QString List::getFilter()
{
    return this->m_filter;
}

//-----------------------------------------------------------------------------

void List::Add(Symbol* symbol)
{
    if (!symbol)
    {
        return;
    }
    auto instance = GetInstance();
    instance->m_allSymbols.append(symbol);
}

//-----------------------------------------------------------------------------

QHash<int, QByteArray> List::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[SymbolRole] = "symbol";

    return roles;
}

//-----------------------------------------------------------------------------

QVariant List::data(const QModelIndex& index, int role) const
{
    QVariant var;

    if (index.isValid())
    {
        if (role == SymbolRole)
        {
            var.setValue(this->m_symbols.at(index.row()));
        }
    }

    return var;
}

//-----------------------------------------------------------------------------

void List::init()
{
    std::sort(this->m_allSymbols.begin(), this->m_allSymbols.end(), [](Symbol* a, Symbol* b) -> const bool
            {
                return a->operator<(* b);
            });

    this->m_symbols = this->m_allSymbols;
}

//-----------------------------------------------------------------------------

int List::rowCount(const QModelIndex&) const
{
    return this->m_symbols.size();
}

//-----------------------------------------------------------------------------

int List::getCount()
{
    return this->m_symbols.count();
}

//-----------------------------------------------------------------------------

int List::getTotalCount()
{
    return this->m_allSymbols.count();
}

//-----------------------------------------------------------------------------

} // namespace TypeInfo
} // namespace fwQtQuick
