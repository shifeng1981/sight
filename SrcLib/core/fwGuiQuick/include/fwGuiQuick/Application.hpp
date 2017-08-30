/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWGUIQUICK_APPLICATION_HPP__
#define __FWGUIQUICK_APPLICATION_HPP__

#include "fwGuiQuick/config.hpp"

#include <fwCore/base.hpp>

#include <fwGui/Application.hpp>

namespace fwGuiQuick
{

/**
 * @brief   Gives access to the qt application part
 *
 */
class FWGUIQUICK_CLASS_API Application : public ::fwGui::Application
{

public:

    Application(::fwGui::GuiBaseObject::Key key)
    {
    }

    virtual ~Application()
    {
    }

    /// Tells the application to exit with a returncode
    FWGUIQUICK_API virtual void exit( int returncode = 0);

};

} // namespace fwGuiQuick

#endif /*__FWGUIQUICK_APPLICATION_HPP__*/

