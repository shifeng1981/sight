/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwGuiQuick/container/QuickContainer.hpp"

namespace fwGuiQuick
{
namespace container
{

//-----------------------------------------------------------------------------

QuickContainer::QuickContainer(::fwGui::GuiBaseObject::Key key) throw()
{
}

//-----------------------------------------------------------------------------

QuickContainer::~QuickContainer() throw()
{
}

//-----------------------------------------------------------------------------

void QuickContainer::clean()
{
}

//-----------------------------------------------------------------------------

void QuickContainer::destroyContainer()
{
}

//-----------------------------------------------------------------------------

bool QuickContainer::isShownOnScreen()
{
    return true;
}

//-----------------------------------------------------------------------------

void QuickContainer::setVisible(bool isVisible)
{
    SLM_WARN("This container cannot be invisible");
}

//-----------------------------------------------------------------------------

void QuickContainer::setEnabled(bool isEnabled)
{
    SLM_WARN("This container cannot be disabled");
}

//-----------------------------------------------------------------------------

} // namespace container
} // namespace fwGuiQuick
