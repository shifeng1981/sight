/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWGUIQUICK_QTMAINFRAME_HPP__
#define __FWGUIQUICK_QTMAINFRAME_HPP__

#include "fwGuiQuick/config.hpp"

#include <boost/function.hpp>

#include <QMainWindow>
#include <QPointer>

namespace fwGuiQuick
{

/**
 * @brief A qt panel used to control a VTK 2D Negatoscope view.
 *
 */
class QtMainFrame : public QMainWindow
{

Q_OBJECT

public:
    /**
     * @brief Constructor.
     */
    FWGUIQUICK_API QtMainFrame() throw();

    /// @brief Destructor.
    FWGUIQUICK_API virtual ~QtMainFrame() throw();

    typedef ::boost::function0<void> CloseCallback;
    FWGUIQUICK_API void setCloseCallback(CloseCallback fct);

protected:
    void closeEvent(QCloseEvent* event);

private:
    CloseCallback m_fctCloseCallback;
};

} // fwGuiQuick

#endif // __FWGUIQUICK_QTMAINFRAME_HPP__
