/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "visuVTKQuick/VtkRenderWindowInteractorManager.hpp"

#include <fwQtQuick/Engine.hpp>
#include <fwQtQuick/render/vtkQuickItem.hpp>

#include <fwRenderVTK/registry/macros.hpp>

#include <vtkObjectFactory.h>

//-----------------------------------------------------------------------------

fwRenderVTKRegisterMacro( ::visuVTKQuick::VtkRenderWindowInteractorManager,
                          ::fwRenderVTK::IVtkRenderWindowInteractorManager::REGISTRY_KEY );

//-----------------------------------------------------------------------------

namespace visuVTKQuick
{

//-----------------------------------------------------------------------------

VtkRenderWindowInteractorManager::VtkRenderWindowInteractorManager(
    ::fwRenderVTK::IVtkRenderWindowInteractorManager::Key key) :
    m_interactor(nullptr)
{
}

//-----------------------------------------------------------------------------

VtkRenderWindowInteractorManager::~VtkRenderWindowInteractorManager()
{
}

//-----------------------------------------------------------------------------

void VtkRenderWindowInteractorManager::installInteractor( ::fwGui::container::fwContainer::sptr _parent )
{
    QObject* object = ::fwQtQuick::Engine::getInstance()->getRootObject();
    const QString itemID = QString::fromStdString(this->m_serviceID);
    ::fwQtQuick::Render::VtkQuickItem* vtkQuickItem = object->findChild< ::fwQtQuick::Render::VtkQuickItem*>(itemID);
    OSLM_ASSERT("vtkQuickItem is null, be sure to set is id as "<<this->m_serviceID<<" in qml file ", vtkQuickItem);
    m_interactor = vtkQuickItem->GetRenderWindow()->GetInteractor();
}

//-----------------------------------------------------------------------------

void VtkRenderWindowInteractorManager::uninstallInteractor()
{
    m_interactor = nullptr;
}

//-----------------------------------------------------------------------------

::vtkRenderWindowInteractor* VtkRenderWindowInteractorManager::getInteractor()
{
    return m_interactor;
}
//-----------------------------------------------------------------------------

} // namespace visuVTKQuick

