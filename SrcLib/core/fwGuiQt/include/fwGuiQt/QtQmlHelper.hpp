/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

# include "fwGuiQt/config.hpp"

# include <QObject>
# include <QQuickWidget>

namespace fwGuiQt
{

class FWGUIQT_CLASS_API QtQmlHelper
{
private:
    // Not instanciable
    FWGUIQT_API QtQmlHelper();
    FWGUIQT_API ~QtQmlHelper();

public:
    /**
     *  @brief: return a QML object from "objectName" field
     */
    static FWGUIQT_API QObject* getObjectByName(std::string const& id);

    /**
     *  @brief: private helper to retrieve the first object of the app i.e the window
     */
    static FWGUIQT_API QQuickWindow* getRootObject();
};

} // fwGuiQt
