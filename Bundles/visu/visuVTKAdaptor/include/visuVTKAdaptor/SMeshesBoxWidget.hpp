/************************************************************************
 *
 * Copyright (C) 2009-2018 IRCAD France
 * Copyright (C) 2012-2018 IHU Strasbourg
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

#pragma once

#include "visuVTKAdaptor/config.hpp"

#include <fwCom/Connection.hpp>
#include <fwCom/Slot.hpp>
#include <fwCom/Slots.hpp>

#include <fwData/Composite.hpp>

#include <fwRenderVTK/IAdaptor.hpp>

class vtkBoxWidget2;
class vtkCommand;
class vtkPolyData;
class vtkAssembly;
class vtkActor;

namespace visuVTKAdaptor
{

/**
 * @brief   Create a Box widget around the meshes contained in the composite.
 *
 * @section Slots Slots
 * - \b updateMatrices() : updates the matrices from the meshes
 * - \b addObject(::fwData::Composite::ContainerType) : updates the box widget with the added meshes
 * - \b changeObject(::fwData::Composite::ContainerType, ::fwData::Composite::ContainerType) : updates the box widget
 *      with the current meshes
 * - \b removeObjects(::fwData::Composite::ContainerType) : updates the box widget with the current meshes
 *
 * @section XML XML Configuration
 *
 * @code{.xml}
   <service type="::visuVTKAdaptor::SMeshesBoxWidget" autoConnect="yes">
       <in key="composite" uid="..." />
       <config renderer="default" />
   </service>
   @endcode
 * @subsection Input Input
 * - \b composite [::fwData::Composite]: Composite containing meshes, the box widget will be around this meshes and when
 *   the box is moved, the meshes transform (in a field) will be modified.
 *
 * @subsection Configuration Configuration
 * - \b config(mandatory) : contains the adaptor configuration
 *    - \b renderer(mandatory) : renderer where the mesh is displayed
 */
class VISUVTKADAPTOR_CLASS_API SMeshesBoxWidget : public ::fwRenderVTK::IAdaptor
{

public:

    fwCoreServiceClassDefinitionsMacro( (SMeshesBoxWidget)(::fwRenderVTK::IAdaptor) );

    VISUVTKADAPTOR_API SMeshesBoxWidget() noexcept;

    VISUVTKADAPTOR_API virtual ~SMeshesBoxWidget() noexcept;

    /// Updates meshes transformation matrix from vtk box widget transform
    void updateFromVtk();

protected:

    VISUVTKADAPTOR_API void configuring() override;
    VISUVTKADAPTOR_API void starting() override;
    VISUVTKADAPTOR_API void updating() override;
    VISUVTKADAPTOR_API void stopping() override;

    /**
     * @brief Returns proposals to connect service slots to associated object signals,
     * this method is used for obj/srv auto connection
     *
     * Connect Composite::s_ADDED_OBJECTS_SIG to this::s_ADD_OBJECTS_SLOT
     * Connect Composite::s_CHANGED_OBJECTS_SIG to this::s_CHANGE_OBJECTS_SLOT
     * Connect Composite::s_REMOVED_OBJECTS_SIG to this::s_REMOVE_OBJECTS_SLOT
     */
    VISUVTKADAPTOR_API virtual KeyConnectionsMap getAutoConnections() const override;

    /// Updates vtk transformation from data meshes
    void updateMeshTransform();

    /// Updates map from composite meshes
    void updateMeshMapFromComposite(::fwData::Composite::ContainerType objects);

private:

    /**
     * @name Slots methods
     * @{
     */
    /// Slot: Updates the matrices
    void updateMatrices();

    /// Slot: add objects
    void addObjects(::fwData::Composite::ContainerType objects);

    /// Slot: change objects
    void changeObjects(::fwData::Composite::ContainerType newObjects, ::fwData::Composite::ContainerType);

    /// Slot: remove objects
    void removeObjects(::fwData::Composite::ContainerType objects);
    /**
     * @}
     */

    typedef std::map< std::string, ::fwCom::Connection > ConnectionMapType;
    typedef std::map< std::string, vtkActor* > MeshMapType;

    MeshMapType m_meshMap;
    ConnectionMapType m_connections;

    vtkAssembly* m_assembly;
    vtkCommand* m_boxWidgetCommand;
    vtkBoxWidget2* m_vtkBoxWidget;
};

} //namespace visuVTKAdaptor
