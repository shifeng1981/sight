/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "visuVTKAdaptor/config.hpp"

#include <fwData/TransferFunction.hpp>

#include <fwDataTools/helper/MedicalImageAdaptor.hpp>

#include <fwRenderVTK/IQAdaptor.hpp>

class vtkCommand;

namespace visuVTKAdaptor
{

/**
 * @brief Manage windowing interaction for image
 *
 * @section XML XML Configuration
 *
 * @code{.xml}
   <service type="::visuVTKAdaptor::SQNegatoWindowingInteractor" autoConnect="yes">
       <inout key="image" uid="..." />
       <inout key="tf" uid="..." optional="yes" />
       <config picker="negatodefault" />
   </service>
   @endcode
 * @subsection In-Out In-Out
 * - \b image [::fwData::Image]: image to display.
 * - \b tf [::fwData::TransferFunction] (optional): the current TransferFunction. If it is not defined, we use the
 *      image's default transferFunction
 *
 * @subsection Configuration Configuration:
 * - \b config(mandatory) : contains the adaptor configuration
 *    - \b picker (mandatory): identifier of the picker
 */
class VISUVTKADAPTOR_CLASS_API SQNegatoWindowingInteractor :    public ::fwRenderVTK::IQAdaptor,
                                                                public ::fwDataTools::helper::MedicalImageAdaptor
{

    Q_OBJECT

    Q_PROPERTY(fwServices::QtObjectHolder *image MEMBER m_image)

public:
    VISUVTKADAPTOR_API SQNegatoWindowingInteractor() noexcept;

    VISUVTKADAPTOR_API virtual ~SQNegatoWindowingInteractor() noexcept;

    VISUVTKADAPTOR_API void startWindowing();

    VISUVTKADAPTOR_API void stopWindowing();

    VISUVTKADAPTOR_API void updateWindowing( double, double);

    VISUVTKADAPTOR_API void resetWindowing();

protected:

    VISUVTKADAPTOR_API void configuring() override;
    VISUVTKADAPTOR_API void starting() override;
    VISUVTKADAPTOR_API void updating() override;
    VISUVTKADAPTOR_API void stopping() override;

    vtkCommand* m_vtkObserver;
    double m_initialWindow;
    double m_initialLevel;
    float m_priority;

private:
    ::fwServices::QtObjectHolder    *m_image = nullptr;
};

} //namespace visuVTKAdaptor

