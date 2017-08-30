/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWGUIQUICK_APP_HPP__
#define __FWGUIQUICK_APP_HPP__

#include "fwGuiQuick/config.hpp"

#include <fwRuntime/profile/Profile.hpp>

#include <QApplication>

namespace fwGuiQuick
{

/**
 * @brief   Defines the Qt application.
 */
class FWGUIQUICK_CLASS_API App : public QApplication
{
Q_OBJECT

public:
    FWGUIQUICK_API App(int& argc, char** argv, bool guiEnabled);

public Q_SLOTS:
    void aboutToQuit();
    void onExit();

};

} // namespace fwGuiQuick

#endif /*__FWGUIQUICK_APP_HPP__*/
