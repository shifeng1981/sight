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

#include <fwData/Image.hpp>

#include <fwDataTools/helper/MedicalImage.hpp>

#include <fwRenderVTK/IAdaptor.hpp>

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
 * @section XML XML Configuration
 *
 * @code{.xml}
   <service type="::visuVTKAdaptor::SNegatoOneSlice" autoConnect="yes">
       <inout key="image" uid="..." />
       <inout key="tf" uid="..." optional="yes" />
       <config renderer="default" picker="negatodefault" sliceIndex="axial"
               transform="trf" tfalpha="yes" interpolation="off" vtkimagesource="imgSource" actorOpacity="1.0" />
   </service>
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
class VISUVTKADAPTOR_CLASS_API SNegatoOneSlice : public ::fwRenderVTK::IAdaptor
{

public:

    fwCoreServiceClassDefinitionsMacro( (SNegatoOneSlice)(::fwRenderVTK::IAdaptor) )

    typedef ::fwDataTools::helper::MedicalImage::Orientation Orientation;

    VISUVTKADAPTOR_API SNegatoOneSlice() noexcept;

    VISUVTKADAPTOR_API virtual ~SNegatoOneSlice() noexcept;

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

    //------------------------------------------------------------------------------

    void setOrientation(int _orientation)
    {
        m_helper.setOrientation(static_cast< Orientation >(_orientation));
    }

protected:

    VISUVTKADAPTOR_API void configuring() override;
    VISUVTKADAPTOR_API void starting() override;
    VISUVTKADAPTOR_API void updating() override;
    VISUVTKADAPTOR_API void stopping() override;
    /// Select the current tf
    VISUVTKADAPTOR_API void swapping(const KeyType& key) override;

    /**
     * @brief Returns proposals to connect service slots to associated object signals,
     * this method is used for obj/srv auto connection
     *
     * Connect Image::s_MODIFIED_SIG to this::s_UPDATE_IMAGE_SLOT
     * Connect Image::s_SLICE_TYPE_MODIFIED_SIG to this::s_UPDATE_SLICE_TYPE_SLOT
     * Connect Image::s_BUFFER_MODIFIED_SIG to this::s_UPDATE_IMAGE_SLOT
     */
    VISUVTKADAPTOR_API virtual KeyConnectionsMap getAutoConnections() const override;

    vtkObject* getImageSource();
    void cleanImageSource();
    ::fwRenderVTK::IAdaptor::sptr getImageSliceAdaptor();
    ::fwRenderVTK::IAdaptor::sptr getImageAdaptor();

    bool m_manageImageSource;
    std::string m_imageSourceId;
    vtkObject* m_imageSource;

    bool m_allowAlphaInTF;
    bool m_interpolation;
    double m_actorOpacity;

    ::fwRenderVTK::IAdaptor::wptr m_imageAdaptor;
    ::fwRenderVTK::IAdaptor::wptr m_imageSliceAdaptor;

private:
    /**
     * @name Slots
     * @{
     */
    /// Slot: update image slice type
    void updateSliceType(int from, int to);

    /// Slot: update image
    void updateImage();
    /**
     * @}
     */

    ::fwDataTools::helper::MedicalImage m_helper;

};

} //namespace visuVTKAdaptor
