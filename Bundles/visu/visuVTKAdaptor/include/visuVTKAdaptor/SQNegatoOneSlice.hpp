/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "visuVTKAdaptor/config.hpp"

#include <fwData/Image.hpp>

#include <fwDataTools/helper/MedicalImageAdaptor.hpp>

#include <fwRenderVTK/IQAdaptor.hpp>

#include <vtkImagePlaneWidget.h>
#include <vtkOutlineFilter.h>

class vtkObject;

namespace visuVTKAdaptor
{

/**
 * @brief Display a negato image with one slice
 *
 * @section Slots Slots
 * - \b updateSliceType(int from, int to): update image slice type
 * - \b updateImage(bool isShown): show/hide slice
 *
 * @section QML QML Configuration
 *
 * @code{.qml}
   SNegatoOneSlice {
        image: ...
        config: ({
            renderer: "default"
            picker: "..."
            sliceIndex: "..."
            transform: "..."
            tfalpha: "..."
            interpolation: "..."
            vtkimagesource: "..."
            actorOpacity: "1.0"
        })

   }
   @endcode
 * @subsection In-Out In-Out
 * - \b image [::fwData::Image]: image to display.
 * - \b tf [::fwData::TransferFunction] (optional): the current TransferFunction. If it is not defined, we use the
 *      image's default transferFunction *
 * @subsection Configuration Configuration:
 * - \b config(mandatory) : contains the adaptor configuration
 *    - \b renderer (mandatory): defines the renderer to show the image.
 *    - \b picker (mandatory): identifier of the picker
 *    - \b sliceIndex (optional, axial/frontal/sagittal, default=axial): orientation of the negato
 *    - \b transform (optional): the vtkTransform to associate to the adaptor
 *    - \b tfalpha (optional, yes/no, default=no): if true, the opacity of the transfer function is used in the negato.
 *    - \b interpolation (optional, yes/no, default=yes): if true, the image pixels are interpolated
 *    - \b vtkimagesource (optional): source image, used for blend
 *    - \b actorOpacity (optional, default=1.0): actor opacity (float)
 */
class VISUVTKADAPTOR_CLASS_API SQNegatoOneSlice :   public ::fwRenderVTK::IQAdaptor,
                                                    public ::fwDataTools::helper::MedicalImageAdaptor
{
Q_OBJECT

Q_PROPERTY(fwServices::QtObjectHolder* image MEMBER m_image)

public:

    VISUVTKADAPTOR_API SQNegatoOneSlice() noexcept;

    VISUVTKADAPTOR_API virtual ~SQNegatoOneSlice() noexcept;

    //------------------------------------------------------------------------------

    void setAllowAlphaInTF(bool allow)
    {
        m_allowAlphaInTF = allow;
    }
    //------------------------------------------------------------------------------

    void setInterpolation(bool interpolation)
    {
        m_interpolation = interpolation;
    }
    //------------------------------------------------------------------------------

    void setVtkImageSourceId(std::string id)
    {
        m_imageSourceId = id;
    }
    //------------------------------------------------------------------------------

    void setVtkImageSource(vtkObject* obj)
    {
        m_imageSource = obj;
    }
    //------------------------------------------------------------------------------

    void setActorOpacity(double actorOpacity)
    {
        m_actorOpacity = actorOpacity;
    }

protected:

    VISUVTKADAPTOR_API void configuring() override;
    VISUVTKADAPTOR_API void starting() override;
    VISUVTKADAPTOR_API void updating() override;
    VISUVTKADAPTOR_API void stopping() override;
    /// Select the current tf
    //VISUVTKADAPTOR_API void swapping(const KeyType& key) override;

    vtkObject* getImageSource();
    void cleanImageSource();
    std::shared_ptr<::fwRenderVTK::IQAdaptor> getImageSliceAdaptor();
    std::shared_ptr<::fwRenderVTK::IQAdaptor> getImageAdaptor();

    bool m_manageImageSource;
    std::string m_imageSourceId;
    vtkObject* m_imageSource;

    bool m_allowAlphaInTF;
    bool m_interpolation;
    double m_actorOpacity;

    std::weak_ptr<::fwRenderVTK::IQAdaptor> m_imageAdaptor;
    std::weak_ptr<::fwRenderVTK::IQAdaptor> m_imageSliceAdaptor;

public:
    void updateSliceIndex(int axial, int frontal, int sagittal);

    /**
     * @name Slots
     * @{
     */
    /// Slot: update image slice type
    void updateSliceType(int from, int to);

private:

    /// Slot: update image
    void updateImage();
    /**
     * @}
     */

    ::fwServices::QtObjectHolder* m_image = nullptr;

    /// @brief image wireframe outline.
    vtkOutlineFilter* m_outline;

    /// @brief vtk widget used in the negatoscope axial plane.
    vtkImagePlaneWidget* m_negatoAxial;

    /// @brief vtk widget used in the negatoscope sagittal plane.
    vtkImagePlaneWidget* m_negatoSagittal;

    /// @brief vtk widget used in the negatoscope frontal plane.
    vtkImagePlaneWidget* m_negatoFrontal;

};

} //namespace visuVTKAdaptor
