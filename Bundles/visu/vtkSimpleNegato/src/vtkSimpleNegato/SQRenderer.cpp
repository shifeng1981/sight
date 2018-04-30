/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2016.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "vtkSimpleNegato/SQRenderer.hpp"

#include <fwCom/Slot.hpp>
#include <fwCom/Slot.hxx>
#include <fwCom/Slots.hpp>
#include <fwCom/Slots.hxx>

#include <fwData/Image.hpp>

#include <fwDataTools/fieldHelper/MedicalImageHelpers.hpp>

#include <fwServices/macros.hpp>

#include <fwVtkIO/vtk.hpp>

#include <fwGuiQt/QtQmlHelper.hpp>
#include <fwGuiQt/container/QtContainer.hpp>
#include <fwGuiQt/QtQmlType.hxx>

#include <vtkCellPicker.h>
#include <vtkCommand.h>
#include <vtkImageData.h>
#include <vtkLookupTable.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkRendererCollection.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkImageActor.h>
#include <vtkGenericDataObjectReader.h>

//-----------------------------------------------------------------------------

namespace vtkSimpleNegato
{

static ::fwGuiQt::QtQmlType<SQRenderer>  registar("com.fw4spl", 1, 0, "SRenderer");

//-----------------------------------------------------------------------------

SQRenderer::SQRenderer() noexcept :
    m_render( 0 ),
    m_target(nullptr),
    m_image(nullptr),
    m_bPipelineIsInit(false)
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

SQRenderer::~SQRenderer() noexcept
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void SQRenderer::configuring()
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void SQRenderer::starting()
{
    SLM_ASSERT("Target not set", m_target);
    SLM_ASSERT("Image not set", m_image);

    m_bPipelineIsInit = false;

    m_render = m_target->getRenderer();
    m_render->SetUseDepthPeeling(1);

}

//-----------------------------------------------------------------------------

void SQRenderer::stopping()
{
    if( m_render == 0 )
    {
        return;
    }

    if(m_bPipelineIsInit)
    {
        m_negatoSagittal->Delete();
        m_negatoFrontal->Delete();
        m_negatoAxial->Delete();
        m_outline->Delete();
    }

    SLM_ASSERT("m_render not instanced", m_render);
    m_render->Delete();
    m_render = 0;

    this->destroy();
}

//-----------------------------------------------------------------------------

void SQRenderer::updating()
{
    refresh();
}

//-----------------------------------------------------------------------------

void SQRenderer::refresh()
{
    SLM_ASSERT("Image not set", m_image->getObject());
    SLM_ASSERT("target not set", m_target);
    bool imageIsValid = ::fwDataTools::fieldHelper::MedicalImageHelpers::checkImageValidity( std::dynamic_pointer_cast<::fwData::Image>(m_image->getObject()) );
    if(imageIsValid )
    {
        if(!m_bPipelineIsInit)
        {
            initVTKPipeline();
            m_bPipelineIsInit = true;
        }
        else
        {
            updateVTKPipeline();
        }

        ::fwData::Image::sptr image = std::dynamic_pointer_cast<::fwData::Image>(m_image->getObject());

        int axialIndex    = static_cast<int>(image->getSize()[2]/2);
        int frontalIndex  = static_cast<int>(image->getSize()[1]/2);
        int sagittalIndex = static_cast<int>(image->getSize()[0]/2);

        m_negatoAxial->SetSliceIndex( axialIndex );
        m_negatoFrontal->SetSliceIndex( frontalIndex );
        m_negatoSagittal->SetSliceIndex( sagittalIndex );
    }
    else
    {
        SLM_FATAL("Image invalid.");
    }
}

//-----------------------------------------------------------------------------

void SQRenderer::initVTKPipeline()
{
    vtkSmartPointer< vtkImageData > vtkImg = vtkSmartPointer< vtkImageData >::New();
    ::fwVtkIO::toVTKImage(std::dynamic_pointer_cast<::fwData::Image>(m_image->getObject()), vtkImg);

    m_outline = vtkOutlineFilter::New();
    m_outline->SetInputData(vtkImg);

    vtkPolyDataMapper *outlineMapper = vtkPolyDataMapper::New();
    outlineMapper->SetInputConnection(m_outline->GetOutputPort());

    vtkActor *outlineActor = vtkActor::New();
    outlineActor->SetMapper( outlineMapper);

    vtkCellPicker *picker = vtkCellPicker::New();
    picker->SetTolerance(0.005);

    //assign default props to the ipw's texture plane actor
    m_negatoSagittal = vtkImagePlaneWidget::New();
    m_negatoSagittal->SetInteractor( m_target->getRenderWindow()->GetInteractor() );
    m_negatoSagittal->SetKeyPressActivationValue('x');
    m_negatoSagittal->SetPicker(picker);
    m_negatoSagittal->GetPlaneProperty()->SetColor(1,0,0);
    m_negatoSagittal->TextureInterpolateOn();
    m_negatoSagittal->SetInputData(vtkImg);
    m_negatoSagittal->SetPlaneOrientationToXAxes();
    m_negatoSagittal->DisplayTextOn();
    m_negatoSagittal->On();
    m_negatoSagittal->InteractionOn();

    m_negatoFrontal = vtkImagePlaneWidget::New();
    m_negatoFrontal->SetInteractor( m_target->getRenderWindow()->GetInteractor() );
    m_negatoFrontal->SetKeyPressActivationValue('y');
    m_negatoFrontal->SetPicker(picker);
    m_negatoFrontal->GetPlaneProperty()->SetColor(0,1,0);
    m_negatoFrontal->TextureInterpolateOn();
    m_negatoFrontal->SetInputData(vtkImg);
    m_negatoFrontal->SetPlaneOrientationToYAxes();
    m_negatoFrontal->SetLookupTable( m_negatoSagittal->GetLookupTable());
    m_negatoFrontal->DisplayTextOn();
    m_negatoFrontal->UpdatePlacement();
    m_negatoFrontal->On();

    m_negatoAxial = vtkImagePlaneWidget::New();
    m_negatoAxial->SetInteractor( m_target->getRenderWindow()->GetInteractor() );
    m_negatoAxial->SetKeyPressActivationValue('z');
    m_negatoAxial->SetPicker(picker);
    m_negatoAxial->GetPlaneProperty()->SetColor(0,0,1);
    m_negatoAxial->TextureInterpolateOn();
    m_negatoAxial->SetInputData(vtkImg);
    m_negatoAxial->SetPlaneOrientationToZAxes();
    m_negatoAxial->SetLookupTable( m_negatoSagittal->GetLookupTable());
    m_negatoAxial->DisplayTextOn();
    m_negatoAxial->On();

    // Add the actors
    m_render->AddActor( outlineActor);

    // Repaint and resize window
    m_render->ResetCamera();

    picker->Delete();
    outlineActor->Delete();
    outlineMapper->Delete();
}

//-----------------------------------------------------------------------------

void    SQRenderer::reconfiguring()
{
}

//-----------------------------------------------------------------------------

void    SQRenderer::destroying()
{
}

//-----------------------------------------------------------------------------

void SQRenderer::updateVTKPipeline()
{
    vtkSmartPointer< vtkImageData > vtkImg = vtkSmartPointer< vtkImageData >::New();
    ::fwVtkIO::toVTKImage(std::dynamic_pointer_cast<::fwData::Image>(m_image->getObject()), vtkImg);

    m_outline->SetInputData(vtkImg);
    m_negatoSagittal->SetInputData(vtkImg);
    m_negatoFrontal->SetInputData(vtkImg);
    m_negatoAxial->SetInputData(vtkImg);
}

//------------------------------------------------------------------------------

}
