/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#ifndef __VISUVTKADAPTOR_SQMaterial_HPP__
#define __VISUVTKADAPTOR_SQMaterial_HPP__

#include "visuVTKAdaptor/config.hpp"

#include <fwRenderVTK/IQAdaptor.hpp>

class VISUVTKADAPTOR_CLASS_API vtkProperty;

namespace fwData
{
class Material;
}

namespace visuVTKAdaptor
{

/**
 * @brief Manage material representation of meshes
 *
 * @section QML QML Configuration
 *
 * @code{.qml}
   import com.fw4spl.vtk.adaptors 1.0

   SMaterial {
        material: ... // QtObjectHolder(::fwData::Material::sptr)
   }
   @endcode
 * @subsection Input Input
 * - \b input [::fwData::Material]: material applied on the mesh.
 */
class VISUVTKADAPTOR_CLASS_API SQMaterial : public ::fwRenderVTK::IQAdaptor
{
Q_OBJECT

Q_PROPERTY(fwServices::QtObjectHolder* material MEMBER m_material)

public:

    static const ::fwServices::IService::KeyType s_MATERIAL_INPUT;

    VISUVTKADAPTOR_API SQMaterial() noexcept;

    VISUVTKADAPTOR_API virtual ~SQMaterial() noexcept;

    VISUVTKADAPTOR_API void setVtkProperty(vtkProperty* property);

    VISUVTKADAPTOR_API vtkProperty* getVtkProperty() const;

protected:

    VISUVTKADAPTOR_API void configuring() override;
    VISUVTKADAPTOR_API void starting() override;
    VISUVTKADAPTOR_API void updating() override;
    VISUVTKADAPTOR_API void stopping() override;

    void updateMaterial( CSPTR(::fwData::Material) material );

    vtkProperty* m_property;
    bool m_manageProperty;
    bool m_lighting;

    ::fwServices::QtObjectHolder* m_material = nullptr;

};

} //namespace visuVTKAdaptor

#endif // __VISUVTKADAPTOR_SQMaterial_HPP__
