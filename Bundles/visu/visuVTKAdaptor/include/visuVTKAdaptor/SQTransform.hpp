/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#ifndef __VISUVTKADAPTOR_SQTransform_HPP__
#define __VISUVTKADAPTOR_SQTransform_HPP__

#include "visuVTKAdaptor/config.hpp"

#include <fwRenderVTK/IQAdaptor.hpp>

#include <vtkSmartPointer.h>

#include <vector>

class vtkTransform;
class vtkCommand;

namespace visuVTKAdaptor
{

/**
 * @brief Adaptor binding a TransformationMatrix3D to a vtkTransform.
 *
 * @section QML QML Configuration
 *
 * @code{.qml}
    STransform {
        tm3d: ...
        config: ({
            transform: ...
            autoRender: "true"
            parent: "parentTransform"
        })
    }
   @endcode
 * @subsection In-Out In-Out
 * - \b tm3d [::fwData::TransformationMatrix3D]: f4s transform matrix.
 *
 * @subsection Configuration Configuration
 *  - \b transform (mandatory) : the vtkTransform to associate to the adaptor
 *  - \b autoRender (optional, "true" by default): if autoRender=true,  the scene is automatically rendered after
 *    doStart, doUpdate, doSwap and doStop if m_vtkPipelineModified=true.
 *  - \b parent (optional): id of the parent vtkTransform, it will be concatenated with this current vtkTransform.
 */
class VISUVTKADAPTOR_CLASS_API SQTransform : public ::fwRenderVTK::IQAdaptor
{
Q_OBJECT

Q_PROPERTY(fwServices::QtObjectHolder* tm3d MEMBER m_tm3d)

public:

    static const ::fwServices::IService::KeyType s_TM3D_INOUT;

    /// Constructor
    VISUVTKADAPTOR_API SQTransform() noexcept;

    /// Destructor
    VISUVTKADAPTOR_API virtual ~SQTransform() noexcept;

    /// Set a new vtkTransform
    VISUVTKADAPTOR_API void setTransform(vtkTransform* t);

    /// Returns the current vtkTransform
    VISUVTKADAPTOR_API vtkTransform* getTransform();

    /// Updates the TransformationMatrix3D from the vtkTransform
    VISUVTKADAPTOR_API void updateFromVtk();

protected:

    VISUVTKADAPTOR_API void configuring() override;

    /// Registers transforms and then updates
    VISUVTKADAPTOR_API void starting() override;

    /// Updates the vtkTransform from the TransformationMatrix3D
    VISUVTKADAPTOR_API void updating() override;

    /// Cleanup
    VISUVTKADAPTOR_API void stopping() override;

    /// Updates
    VISUVTKADAPTOR_API void swapping();

    /// Current vtkTransform
    vtkTransform* m_transform;

    /// Command used to listen m_transform changes
    vtkCommand* m_transformCommand;

    /// Parent vtkTransform id (optional)
    std::string m_parentId;

    /// Parent vtkTransform (optional)
    vtkSmartPointer<vtkTransform> m_parentTransform;

    ::fwServices::QtObjectHolder* m_tm3d = nullptr;
};

} //namespace visuVTKAdaptor

#endif // __VISUVTKADAPTOR_SQTransform_HPP__
