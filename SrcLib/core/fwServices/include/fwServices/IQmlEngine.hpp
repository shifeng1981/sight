/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "fwServices/config.hpp"

namespace fwServices
{

class FWSERVICES_CLASS_API IQmlEngine
{
public:
    // dtor, do nothing
    virtual FWSERVICES_API ~IQmlEngine();

    /**
     *  @brief: This method load a script with his path specified by filePath.
     *
     *  If the script can't be loaded, an exception is throw.
     */
    virtual void FWSERVICES_API		loadFile(std::string const& filePath) = 0;

    /**
     *  @brief: This will start the window and run the program.
     */
    virtual void FWSERVICES_API		launch() = 0;
    /**
     *  @brief: Allow to create pre-context object to be passed from C++ to QML.
     *  This method is not recommanded to use. Please prefer to create your object directly from QML.
     */
    virtual void FWSERVICES_API		addCtx(std::string const& uid, std::string const& type) = 0;

    /**
     *  @brief: This method stop the application and QApplication will exit with "status" code.
     */
    virtual void FWSERVICES_API     stop(int status) = 0;
};

}
