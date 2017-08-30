/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWGUIQUICK_ACTIONCALLBACK_HPP__
#define __FWGUIQUICK_ACTIONCALLBACK_HPP__

#include "fwGuiQuick/config.hpp"

#include <fwGui/ActionCallbackBase.hpp>

#include <QObject>

namespace fwGuiQuick
{

/**
 * @brief   Defines the menu item callback.
 *
 */
class FWGUIQUICK_CLASS_API ActionCallback :  public QObject,
                                             public ::fwGui::ActionCallbackBase
{

Q_OBJECT

public:

    fwCoreClassDefinitionsWithFactoryMacro( (ActionCallback)(::fwGui::ActionCallbackBase),
                                            (()),
                                            ::fwGui::factory::New< ActionCallback > );

    FWGUIQUICK_API ActionCallback(::fwGui::GuiBaseObject::Key key);

    FWGUIQUICK_API virtual ~ActionCallback();

    /**
     * @brief  Qt slot.
     */
public Q_SLOTS:
    void executeQt(bool checked = false);
    void checkQt(bool checked);

};

} // namespace fwGuiQuick

#endif /*__FWGUIQUICK_ACTIONCALLBACK_HPP__*/

