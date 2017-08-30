/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWGUIQUICK_WORKERQT_HPP__
#define __FWGUIQUICK_WORKERQT_HPP__

#include "fwGuiQuick/config.hpp"

#include <fwCore/base.hpp>

namespace fwThread
{
class Worker;
} //namespace fwThread

namespace fwGuiQuick
{

//-----------------------------------------------------------------------------

FWGUIQUICK_API SPTR(::fwThread::Worker) getQtWorker(int& argc, char** argv, bool guiEnabled = true);

//-----------------------------------------------------------------------------

} //namespace fwGui

#endif /*__FWGUIQUICK_WORKERQT_HPP__*/

