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

#ifndef __FWGUIQT_ACTIONCALLBACK_HPP__
#define __FWGUIQT_ACTIONCALLBACK_HPP__


#include <QObject>

#include <fwGui/ActionCallbackBase.hpp>

#include "fwGuiQt/config.hpp"


namespace fwGuiQt
{

/**
 * @brief   Defines the menu item callback.
 * @class   ActionCallback
 *
 * @date    2009-2010.
 *
 */
class FWGUIQT_CLASS_API ActionCallback :  public QObject,
                                          public ::fwGui::ActionCallbackBase
{

Q_OBJECT

public:

    fwCoreClassDefinitionsWithFactoryMacro( (ActionCallback)(::fwGui::ActionCallbackBase),
                                            (()),
                                            ::fwGui::factory::New< ActionCallback > );

    FWGUIQT_API ActionCallback(::fwGui::GuiBaseObject::Key key);

    FWGUIQT_API virtual ~ActionCallback();

    /**
     * @brief  Qt slot.
     */
public Q_SLOTS:
    void executeQt(bool checked = false);
    void checkQt(bool checked);

};

} // namespace fwGuiQt

#endif /*__FWGUIQT_ACTIONCALLBACK_HPP__*/


