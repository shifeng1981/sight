/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwGuiQuick/builder/ContainerBuilder.hpp"

#include "fwGuiQuick/container/QuickContainer.hpp"

#include <fwGui/registry/macros.hpp>

fwGuiRegisterMacro(::fwGui::builder::ContainerBuilder, ::fwGui::builder::IContainerBuilder::REGISTRY_KEY);

namespace fwGui
{
namespace builder
{

//-----------------------------------------------------------------------------

ContainerBuilder::ContainerBuilder(::fwGui::GuiBaseObject::Key key)
{
}

//-----------------------------------------------------------------------------

ContainerBuilder::~ContainerBuilder()
{
}

//-----------------------------------------------------------------------------

void ContainerBuilder::createContainer( ::fwGui::container::fwContainer::sptr parent )
{
    ::fwGuiQuick::container::QuickContainer::sptr quickContainer = ::fwGuiQuick::container::QuickContainer::New();
    m_container                                                  = quickContainer;
}

//-----------------------------------------------------------------------------

void ContainerBuilder::destroyContainer()
{
    SLM_ASSERT("Sorry, Container not initialized", m_container);
    m_container->destroyContainer();
}

//-----------------------------------------------------------------------------

} // namespace builder
} // namespace fwGui
