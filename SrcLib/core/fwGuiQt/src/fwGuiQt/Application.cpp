/************************************************************************
 *
 * Copyright (C) 2009-2015 IRCAD France
 * Copyright (C) 2012-2015 IHU Strasbourg
 *
 * This file is part of Sight.
 *
 * Sight is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Sight. If not, see <https://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#include <QApplication>

#include <fwGui/dialog/IMessageDialog.hpp>
#include <fwGui/dialog/MessageDialog.hpp>
#include <fwGui/registry/macros.hpp>

#include "fwGuiQt/Application.hpp"


fwGuiRegisterMacro(::fwGuiQt::Application, ::fwGui::IApplication::REGISTRY_KEY );

namespace fwGuiQt
{

//-----------------------------------------------------------------------------

void Application::exit(int returncode)
{
    if(m_confirm)
    {
        ::fwGui::dialog::MessageDialog dlg;
        dlg.setTitle("Confirm exit");
        dlg.setMessage("Do you really want to exit?");
        dlg.setIcon(::fwGui::dialog::IMessageDialog::QUESTION);
        dlg.addButton(::fwGui::dialog::IMessageDialog::YES_NO);

        ::fwGui::dialog::IMessageDialog::Buttons res = dlg.show();

        if(res == ::fwGui::dialog::IMessageDialog::YES)
        {
            qApp->exit(returncode);
        }
    }
    else
    {
        qApp->exit(returncode);
    }
}

//-----------------------------------------------------------------------------

} // namespace fwGuiQt



