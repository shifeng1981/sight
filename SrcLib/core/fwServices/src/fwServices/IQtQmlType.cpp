/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwServices/IQtQmlType.hpp"

namespace fwServices
{

IQtQmlType::ClassList IQtQmlType::m_classList;

IQtQmlType::IQtQmlType()
{
    m_classList.push_back(this);
}

IQtQmlType::~IQtQmlType()
{
}

//------------------------------------------------------------------------------

void IQtQmlType::registarAllClasses()
{
    auto iterator = m_classList.begin();

    while (iterator != m_classList.end())
    {
        (*iterator)->registar();
        iterator = m_classList.erase(iterator);
    }
}

}
