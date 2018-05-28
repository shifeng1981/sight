/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwServices/IQtQmlObject.hpp"

namespace fwServices
{

IQtQmlObject::IQtQmlObject(std::string const& cType)
{
    m_cType = cType;
}

IQtQmlObject::~IQtQmlObject()
{
}

//------------------------------------------------------------------------------

std::string const&	IQtQmlObject::getClassType() const
{
    return m_cType;
}

} // fwGuiQt
