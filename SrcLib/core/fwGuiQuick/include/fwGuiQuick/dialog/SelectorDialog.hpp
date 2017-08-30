/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWGUIQUICK_DIALOG_SELECTORDIALOG_HPP__
#define __FWGUIQUICK_DIALOG_SELECTORDIALOG_HPP__

#include "fwGuiQuick/config.hpp"

#include <fwGui/dialog/ISelectorDialog.hpp>

#include <vector>

namespace fwGuiQuick
{
namespace dialog
{
//------------------------------------------------------------------------------

/**
 * @brief   SelectorDialog allowing the choice of an element among severals (_selections)
 */
class FWGUIQUICK_CLASS_API SelectorDialog : public ::fwGui::dialog::ISelectorDialog
{
public:

    fwCoreClassDefinitionsWithFactoryMacro( (SelectorDialog)(::fwGui::dialog::ISelectorDialog),
                                            (()),
                                            ::fwGui::factory::New< SelectorDialog > );

    FWGUIQUICK_API SelectorDialog(::fwGui::GuiBaseObject::Key key);

    FWGUIQUICK_API virtual ~SelectorDialog();

    /**
     * @brief The string list that can be chosen by the selector.
     */
    FWGUIQUICK_API virtual void setSelections(std::vector< std::string > _selections);

    /**
     * @brief Sets the selector title.
     */
    FWGUIQUICK_API virtual void setTitle(std::string _title);

    /**
     * @brief Show the selector and return the selection.
     */
    FWGUIQUICK_API virtual std::string show();

    /// Set the message
    FWGUIQUICK_API virtual void setMessage(const std::string& msg);

private:

    std::vector< std::string > m_selections;

    /// Dialog box message
    std::string m_message;
    std::string m_title;
};

} // namespace dialog
} // namespace fwGuiQuick

#endif /*__FWGUIQUICK_DIALOG_SELECTORDIALOG_HPP__*/
