/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "fwTools/config.hpp"

#include <string>
#include <vector>

namespace fwTools
{

/**
 * @brief   Namespace fwTools::os contains tools methods which depend on os like get user application data directory.
 *
 */
namespace os
{

/**
 * @brief Returns a environment variable value
 *
 * @param name environment variable 'name'
 * @param[out] ok boolean set to true if variable exists
 *
 * @return The environment variable content if it exists, else an empty string
 */
FWTOOLS_API std::string getEnv(const std::string& name, bool* ok = NULL);

/**
 * @brief Returns a environment variable value
 *
 * @param name environment variable 'name'
 * @param defaultValue Value returned if variable 'name' doesn't exist
 */
FWTOOLS_API std::string getEnv(const std::string& name, const std::string& defaultValue);

/**
 * @brief   Return the users's application data directory
 *
 * @param company The company name
 * @param appName The application name
 * @param createDirectory if true, create the returned directory if it don't exist
 *
 * Return the application data directory. If company or appName is not empty, append them to
 * the path. Under unix, XDG conventions are repected.
 * For example, the UserDataDir under linux will be "~/.config/company/appName"
 */
FWTOOLS_API std::string  getUserDataDir(
    std::string company  = "",
    std::string appName  = "",
    bool createDirectory = false
    );

/**
 * @brief getDeviceFromVirtualDevice returns the real path of a usb video device (Only Linux for now).
 * @param _virtualDeviceUri: the virtual device uri (/dev/video# on linux, ...)
 * @return std::vector< std::pair< int, std::string> >: vector of pair of device number (int) & system path (string)
 *  of given virtual device uri.
 * The vector is usualy of size 1, but if associated usb device is found  (such as depth sensor on a RGBD camera)
 * the vector can be larger.
 * vector[0] = usb device number and system path corresponding to given virtual device URI.
 * vector[1-N] = all related usb device number and its system path.
 */
FWTOOLS_API std::vector< std::pair< int, std::string > > getDeviceFromVirtualDevice(std::string _virtualDeviceUri);

} // namespace os

} // namespace fwTools
