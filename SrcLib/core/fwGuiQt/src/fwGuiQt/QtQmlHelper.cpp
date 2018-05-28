/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwGuiQt/QtQmlHelper.hpp"

#include "fwGuiQt/QtQmlEngine.hpp"

#include <QQuickItem>

namespace fwGuiQt
{

QtQmlHelper::QtQmlHelper()
{
}

//------------------------------------------------------------------------------

QObject* QtQmlHelper::getObjectByName(std::string const& id)
{
    return getRootObject()->findChild<QObject*>(id.c_str());
}

//------------------------------------------------------------------------------

QQuickWindow* QtQmlHelper::getRootObject()
{
    return QtQmlEngine::getEngine().getWindow();
}

QtQmlHelper::~QtQmlHelper()
{
}

} // fwGuiQt
