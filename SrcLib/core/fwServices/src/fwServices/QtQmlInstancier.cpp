/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwServices/QtQmlInstancier.hxx"

#include <algorithm>

namespace fwServices
{

std::vector<IQtQmlObject::sptr >	QtQmlInstancier::m_classList;

//------------------------------------------------------------------------------

void QtQmlInstancier::addClass(std::shared_ptr<IQtQmlObject> const& object)
{
    m_classList.push_back(object);
}

//------------------------------------------------------------------------------

QObject* QtQmlInstancier::instanciate(std::string const& cType)
{
    auto iterator = m_classList.begin();

    while (iterator != m_classList.end())
    {
        if ((*iterator)->getClassType().compare(cType) == 0)
        {
            return (*iterator)->instanciate();
        }
        ++iterator;
    }
    return nullptr;
}

} // fwGuiQt
