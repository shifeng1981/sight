/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWGUIQUICK_DIALOG_MULTISELECTORDIALOG_HPP__
#define __FWGUIQUICK_DIALOG_MULTISELECTORDIALOG_HPP__

#include "fwGuiQuick/config.hpp"

#include <fwGui/dialog/IMultiSelectorDialog.hpp>

#include <QDialog>
#include <QString>
#include <QWidget>

#include <vector>

namespace fwGuiQuick
{
namespace dialog
{
//------------------------------------------------------------------------------

/**
 * @brief   MultiSelectorDialog allowing the choice of an element among severals (_selections)
 */

class FWGUIQUICK_CLASS_API MultiSelectorDialog : public ::fwGui::dialog::IMultiSelectorDialog,
                                                 public QDialog
{
public:

    fwCoreClassDefinitionsWithFactoryMacro( (MultiSelectorDialog)(::fwGui::dialog::IMultiSelectorDialog),
                                            (()),
                                            ::fwGui::factory::New< MultiSelectorDialog > );

    FWGUIQUICK_API MultiSelectorDialog(::fwGui::GuiBaseObject::Key key);

    FWGUIQUICK_API virtual ~MultiSelectorDialog();

    /**
     * @brief The string list that can be chosen by the selector.
     */
    FWGUIQUICK_API virtual void setSelections(Selections _selections);

    /**
     * @brief Sets the selector title.
     */
    FWGUIQUICK_API virtual void setTitle(std::string _title);

    /**
     * @brief Show the selector and return the selection.
     */
    FWGUIQUICK_API virtual Selections show();

    FWGUIQUICK_API virtual void setMessage(const std::string& msg);

private:

    Selections m_selections;
    /// Dialog box message
    std::string m_message;
    std::string m_title;
};

} // namespace dialog
} // namespace fwGuiQuick

#endif /*__FWGUIQUICK_DIALOG_MULTISELECTORDIALOG_HPP__*/
