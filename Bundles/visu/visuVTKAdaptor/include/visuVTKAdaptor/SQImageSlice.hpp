/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "visuVTKAdaptor/config.hpp"

#include <fwCom/Connection.hpp>
#include <fwCom/helper/SigSlotConnection.hpp>

#include <fwData/Composite.hpp>
#include <fwData/Image.hpp>

#include <fwDataTools/helper/MedicalImageAdaptor.hpp>

#include <fwRenderVTK/IQAdaptor.hpp>

class vtkImageActor;
class vtkLookupTable;
class vtkImageMapToColors;
class vtkImageData;
class vtkPolyDataMapper;
class vtkPolyData;
class vtkActor;

namespace visuVTKAdaptor
{

/**
 * @brief Adaptor to display only one slice of an image
 *
 *
 * @section Slots Slots
 * - \b updateSliceIndex() : update image slice index
 * - \b updateSliceType() : update image slice type
 *
 * @section XML XML Configuration
 *
 * @code{.xml}
   <service type="::visuVTKAdaptor::SImageSlice" autoConnect="yes">
       <inout key="image" uid="..." />
       <config renderer="default" picker="negatodefault" sliceIndex="axial"
               transform="trf" ctrlimage="imageKey" interpolation="off" vtkimagesource="imgSource"
               actorOpacity="1.0" />
   </service>
   @endcode
 *
 * @subsection In-Out In-Out
 * - \b image [::fwData::Image]: image to display.
 *
 * @subsection Configuration Configuration
 * - \b config(mandatory) : contains the adaptor configuration
 *    - \b renderer (mandatory): defines the renderer to show the image
 *    - \b picker (mandatory): identifier of the picker
 *    - \b sliceIndex (optional, axial/frontal/sagittal, default=axial): orientation of the negato
 *    - \b transform (optional): the vtkTransform to associate to the adaptor
 *    - \b ctrlimage (mandatory): image to show
 *    - \b interpolation (optional, yes/no, default=yes): if true, the image pixels are interpolated
 *    - \b vtkimagesource (optional): source image, used for blend
 *    - \b actorOpacity (optional, default=1.0): actor opacity (float)
 */

class VISUVTKADAPTOR_CLASS_API SQImageSlice :  public ::fwRenderVTK::IQAdaptor,
                                               public ::fwDataTools::helper::MedicalImageAdaptor
{

    Q_OBJECT

    Q_PROPERTY(fwServices::QtObjectHolder *image MEMBER m_image)

public:

    VISUVTKADAPTOR_API SQImageSlice() noexcept;

    VISUVTKADAPTOR_API virtual ~SQImageSlice() noexcept;

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

    void setInterpolation(bool interpolation)
    {
        m_interpolation = interpolation;
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

    virtual void buildPipeline();
    virtual void buildOutline();

    void setSlice( int slice, ::fwData::Image::sptr image  );

    ::fwData::Image::sptr getCtrlImage();

    void updateOutline();
    void updateImage( ::fwData::Image::sptr SImageSlice  );
    void updateSImageSliceIndex( ::fwData::Image::sptr SImageSlice );

    bool m_interpolation;
    double m_actorOpacity;

    std::string m_imageSourceId;
    vtkObject* m_imageSource;

    vtkImageActor* m_imageActor;

    vtkPolyData* m_planeOutlinePolyData;
    vtkPolyDataMapper* m_planeOutlineMapper;
    vtkActor* m_planeOutlineActor;

public:
    void updateSliceIndex(int axial, int frontal, int sagittal);


    /**
     * @name Slots
     * @{
     */

    /// Slot: update image slice index

    /// Slot: update image slice type
    void updateSliceType(int from, int to);
    /**
     * @}
     */
private:

    ::fwServices::QtObjectHolder  *m_image = nullptr;

};

} //namespace visuVTKAdaptor
