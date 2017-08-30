/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWGUIQUICK_CURSOR_HPP__
#define __FWGUIQUICK_CURSOR_HPP__

#include "fwGuiQuick/config.hpp"

#include <fwCore/base.hpp>

#include <fwGui/ICursor.hpp>

namespace fwGuiQuick
{

/**
 * @brief   Defines the generic cursor for IHM.
 *
 */
class FWGUIQUICK_CLASS_API Cursor : public ::fwGui::ICursor
{

public:

    fwCoreClassDefinitionsWithFactoryMacro( (Cursor)(::fwGui::ICursor),
                                            (()),
                                            ::fwGui::factory::New< Cursor > );

    FWGUIQUICK_API Cursor(::fwGui::GuiBaseObject::Key key);

    FWGUIQUICK_API virtual ~Cursor();

    /// Set the cursor
    FWGUIQUICK_API virtual void setCursor( ::fwGui::ICursor::CursorType cursor);

    /// Set the default cursor
    FWGUIQUICK_API virtual void setDefaultCursor();

};

} // namespace fwGuiQuick

#endif /*__FWGUIQUICK_CURSOR_HPP__*/

