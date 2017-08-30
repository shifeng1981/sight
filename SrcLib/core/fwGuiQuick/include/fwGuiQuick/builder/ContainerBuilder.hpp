/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWGUIQUICK_BUILDER_CONTAINERBUILDER_HPP__
#define __FWGUIQUICK_BUILDER_CONTAINERBUILDER_HPP__

#include "fwGuiQuick/config.hpp"
#include "fwGuiQuick/container/QuickContainer.hpp"

#include <fwCore/base.hpp>

#include <fwGui/builder/IContainerBuilder.hpp>
#include <fwGui/container/fwContainer.hpp>

#include <fwRuntime/ConfigurationElement.hpp>

namespace fwGui
{
namespace builder
{

/**
 * @brief   Defines a builder for dummy container.
 */
class FWGUIQUICK_CLASS_API ContainerBuilder : public ::fwGui::builder::IContainerBuilder
{
public:
    fwCoreClassDefinitionsWithFactoryMacro( (ContainerBuilder)(::fwGui::builder::IContainerBuilder),
                                            (()),
                                            ::fwGui::factory::New< ContainerBuilder > );

    FWGUIQUICK_API ContainerBuilder(::fwGui::GuiBaseObject::Key key);

    FWGUIQUICK_API virtual ~ContainerBuilder();

    /**
     * @brief Instantiate layout with parent container.
     * @pre LayoutManager must be initialized before.
     * @pre parent container must be instanced.
     */
    FWGUIQUICK_API virtual void createContainer( ::fwGui::container::fwContainer::sptr parent = nullptr );

    /**
     * @brief Destroy local layout with sub containers.
     * @pre services using this sub containers must be stopped before.
     */
    FWGUIQUICK_API virtual void destroyContainer();

    //------------------------------------------------------------------------------

    virtual void setParent(::fwGui::container::fwContainer::sptr parent)
    {
    }

protected:
    ::fwGuiQuick::container::QuickContainer::sptr m_parent;
};

} // namespace builder
} // namespace fwGui

#endif /*__FWGUIQUICK_BUILDER_CONTAINERBUILDER_HPP__*/
