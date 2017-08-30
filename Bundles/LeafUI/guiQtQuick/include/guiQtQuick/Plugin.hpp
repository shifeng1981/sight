/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __GUIQTQUICK_PLUGIN_HPP__
#define __GUIQTQUICK_PLUGIN_HPP__

#include "guiQtQuick/config.hpp"

#include <fwRuntime/Plugin.hpp>

namespace fwThread
{
class Worker;
} //namespace fwThread

namespace guiQtQuick
{

class GUIQTQUICK_CLASS_API Plugin : public ::fwRuntime::Plugin
{

public:

    /**
     * @brief   destructor
     */
    GUIQTQUICK_API ~Plugin() throw();

    // Overrides
    GUIQTQUICK_API void start() throw(::fwRuntime::RuntimeException);

    // Overrides
    GUIQTQUICK_API void stop() throw();

    GUIQTQUICK_API int run() throw();

protected:

    GUIQTQUICK_API void loadStyleSheet();

private:

    SPTR(::fwThread::Worker) m_workerQt;

};

} // namespace guiQtQuick

#endif //__GUIQTQUICK_PLUGIN_HPP__
