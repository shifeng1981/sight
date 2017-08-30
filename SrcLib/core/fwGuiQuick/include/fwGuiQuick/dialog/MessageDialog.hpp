/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWGUIQUICK_DIALOG_MESSAGEDIALOG_HPP__
#define __FWGUIQUICK_DIALOG_MESSAGEDIALOG_HPP__

#include "fwGuiQuick/config.hpp"

#include <fwCore/base.hpp>

#include <fwGui/dialog/IMessageDialog.hpp>

#include <QVector>

#include <string>

class QPushButton;

namespace fwGuiQuick
{
namespace dialog
{

/**
 * @brief Defines the generic message box for IHM.
 */
class FWGUIQUICK_CLASS_API MessageDialog : public ::fwGui::dialog::IMessageDialog
{

public:

    fwCoreClassDefinitionsWithFactoryMacro( (MessageDialog)(::fwGui::dialog::IMessageDialog),
                                            (()),
                                            ::fwGui::factory::New< MessageDialog > );

    FWGUIQUICK_API MessageDialog(::fwGui::GuiBaseObject::Key key);

    FWGUIQUICK_API virtual ~MessageDialog();

    /// Set the title of the message box
    FWGUIQUICK_API virtual void setTitle( const std::string& title );

    /// Set the message
    FWGUIQUICK_API virtual void setMessage( const std::string& msg );

    /// Set the icon (CRITICAL, WARNING, INFO or QUESTION)
    FWGUIQUICK_API virtual void setIcon( IMessageDialog::Icons icon);

    /// Add a button (OK, YES_NO, CANCEL)
    FWGUIQUICK_API virtual void addButton( IMessageDialog::Buttons button );

    /// Add a custom button to this dialog
    FWGUIQUICK_API void addCustomButton(QPushButton* button);

    /// Set the default buttons
    FWGUIQUICK_API virtual void setDefaultButton( IMessageDialog::Buttons button );

    /// Show the message box and return the clicked button.
    FWGUIQUICK_API virtual Buttons show();

protected:

    /// Dialog title
    std::string m_title;

    /// Dialog box message
    std::string m_message;

    /// List of the button
    ::fwGui::dialog::IMessageDialog::Buttons m_buttons;

    /// default buttons
    ::fwGui::dialog::IMessageDialog::Buttons m_defaultButton;

    /// Icon
    ::fwGui::dialog::IMessageDialog::Icons m_icon;

    /// Stores custom buttons
    QVector< QPushButton* > m_customButtons;
};

} // namespace dialog
} // namespace fwGuiQuick

#endif /* __FWGUIQUICK_DIALOG_MESSAGEDIALOG_HPP__ */

