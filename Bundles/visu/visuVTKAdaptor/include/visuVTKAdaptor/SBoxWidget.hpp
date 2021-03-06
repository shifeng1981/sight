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

#include <fwRenderVTK/IAdaptor.hpp>

class vtkTransform;
class vtkBoxWidget2;
class vtkCommand;

namespace visuVTKAdaptor
{

/**
 * @brief Displays a box widget that updates the associated TransformationMatrix3D
 *
 * @section XML XML Configuration
 *
 * @code{.xml}
   <service uid="..." type="::visuVTKAdaptor::SBoxWidget" autoConnect="yes">
       <inout key="transform" uid="..." />
       <config renderer="default" transform="..." scaleFactor="..." enableScaling="no" />
   </service>
   @endcode
 * @subsection In-Out In-Out
 * - \b transform [::fwData::TransformationMatrix3D]: matrix updated by the widget.
 * @subsection Configuration Configuration
 * - \b config(mandatory) : contains the adaptor configuration
 *    - \b renderer (mandatory): defines the renderer to use.
 *    - \b transform (mandatory): the vtkTransform to associate to the adaptor
 *    - \b scaleFactor (optional, default: 1): scale factor of the widget
 *    - \b enableScaling (optional, default: false): if true, active the scaling
 */
class VISUVTKADAPTOR_CLASS_API SBoxWidget : public ::fwRenderVTK::IAdaptor
{
public:

    fwCoreServiceClassDefinitionsMacro( (SBoxWidget)(::fwRenderVTK::IAdaptor) );

    VISUVTKADAPTOR_API SBoxWidget() noexcept;
    VISUVTKADAPTOR_API virtual ~SBoxWidget() noexcept;

    VISUVTKADAPTOR_API void updateFromVtk();

protected:

    VISUVTKADAPTOR_API void configuring() override;
    VISUVTKADAPTOR_API void starting() override;
    VISUVTKADAPTOR_API void updating() override;
    VISUVTKADAPTOR_API void stopping() override;

    /**
     * @brief Returns proposals to connect service slots to associated object signals,
     * this method is used for obj/srv auto connection
     *
     * Connect TransformationMatrix3D::s_MODIFIED_SIG to this::s_UPDATE_SLOT
     */
    VISUVTKADAPTOR_API virtual KeyConnectionsMap getAutoConnections() const override;

    vtkTransform* m_transform;

private:

    vtkBoxWidget2* m_vtkBoxWidget;
    vtkCommand* m_boxWidgetCommand;

    double m_scaleFactor;
    bool m_enableScaling;
};

} // namespace visuVTKAdaptor
