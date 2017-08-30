/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWGUIQUICK_CONTAINER_QUICKCONTAINER_HPP__
#define __FWGUIQUICK_CONTAINER_QUICKCONTAINER_HPP__

#include "fwGuiQuick/config.hpp"

#include <fwCore/base.hpp>

#include <fwGui/container/fwContainer.hpp>

#include <fwTools/Object.hpp>

namespace fwGuiQuick
{
namespace container
{

/**
 * @brief   Defines the android container for IHM.
 */
class FWGUIQUICK_CLASS_API QuickContainer : public ::fwGui::container::fwContainer
{

public:

    fwCoreClassDefinitionsWithFactoryMacro( (QuickContainer)(::fwGui::container::fwContainer),
                                            (()),
                                            ::fwGui::factory::New< QuickContainer > );

    FWGUIQUICK_API QuickContainer(::fwGui::GuiBaseObject::Key key) throw();

    FWGUIQUICK_API virtual ~QuickContainer() throw();

    FWGUIQUICK_API virtual void clean();
    FWGUIQUICK_API virtual void destroyContainer();

    FWGUIQUICK_API virtual bool isShownOnScreen();

    FWGUIQUICK_API virtual void setVisible(bool isVisible);
    FWGUIQUICK_API virtual void setEnabled(bool isEnabled);
};

} // namespace container
} // namespace fwGuiQuick

#endif /*__FWGUIQUICK_CONTAINER_QUICKCONTAINER_HPP__*/
