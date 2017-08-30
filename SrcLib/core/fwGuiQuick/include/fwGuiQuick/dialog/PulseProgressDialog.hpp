/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWGUIQUICK_DIALOG_PULSEPROGRESSDIALOG_HPP__
#define __FWGUIQUICK_DIALOG_PULSEPROGRESSDIALOG_HPP__

#include "fwGuiQuick/config.hpp"

#include <fwGui/dialog/IPulseProgressDialog.hpp>

#include <QPointer>
#include <QProgressDialog>

#include <string>

QT_BEGIN_NAMESPACE
class QProgressDialog;
QT_END_NAMESPACE

namespace fwGuiQuick
{
namespace dialog
{
/**
 * @brief   This class allows us to show a pulse progress bar.
 */
class FWGUIQUICK_CLASS_API PulseProgressDialog : public ::fwGui::dialog::IPulseProgressDialog
{
public:

    fwCoreClassDefinitionsWithFactoryMacro( (PulseProgressDialog)(::fwGui::dialog::IPulseProgressDialog),
                                            (()),
                                            ::fwGui::factory::New< PulseProgressDialog > );

    FWGUIQUICK_API PulseProgressDialog(::fwGui::GuiBaseObject::Key key);

    FWGUIQUICK_API virtual ~PulseProgressDialog();

    /// override
    FWGUIQUICK_API void setTitle(const std::string& title);

    /// override
    FWGUIQUICK_API void setMessage(const std::string& message);

    FWGUIQUICK_API void show();

protected:

    QPointer< QProgressDialog >  m_pdialog;

};
} // namespace dialog
} // namespace fwGuiQuick

#endif /* __FWGUIQUICK_DIALOG_PULSEPROGRESSDIALOG_HPP__ */
