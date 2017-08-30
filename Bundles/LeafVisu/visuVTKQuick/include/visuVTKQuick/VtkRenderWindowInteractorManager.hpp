/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __VISUVTKQUICK_VTKRENDERWINDOWINTERACTORMANAGER_HPP__
#define __VISUVTKQUICK_VTKRENDERWINDOWINTERACTORMANAGER_HPP__

#include "visuVTKQuick/config.hpp"

#include <fwCore/base.hpp>

#include <fwRenderVTK/IVtkRenderWindowInteractorManager.hpp>

namespace visuVTKQuick
{

/**
 * @brief   Defines a class to manage vtkRenderWindowInteractor in a window.
 */
class VISUVTKQUICK_CLASS_API VtkRenderWindowInteractorManager : public ::fwRenderVTK::IVtkRenderWindowInteractorManager
{

public:

    fwCoreNonInstanciableClassDefinitionsMacro((VtkRenderWindowInteractorManager)
                                               (::fwRenderVTK::IVtkRenderWindowInteractorManager))

    VISUVTKQUICK_API VtkRenderWindowInteractorManager(::fwRenderVTK::IVtkRenderWindowInteractorManager::Key key);

    VISUVTKQUICK_API virtual ~VtkRenderWindowInteractorManager();

    /// Creates an interactor and installs it in window.
    VISUVTKQUICK_API virtual void installInteractor( ::fwGui::container::fwContainer::sptr _parent );

    /// Deletes interactor and manage correctly the window (removing layout).
    VISUVTKQUICK_API virtual void uninstallInteractor();

    /// Return a pointer on interactor
    VISUVTKQUICK_API virtual ::vtkRenderWindowInteractor* getInteractor();

private:
    vtkRenderWindowInteractor* m_interactor;

};

} // namespace visuVTKQuick

#endif // __VISUVTKQUICK_VTKRENDERWINDOWINTERACTORMANAGER_HPP__

