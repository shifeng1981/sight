/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "visuVTKAdaptor/SQMedical3DCamera.hpp"

#include <fwCom/Slot.hxx>
#include <fwCom/Slots.hxx>

#include <fwData/String.hpp>
#include <fwData/TransformationMatrix3D.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/QtQmlRegistar.hxx>

#include <boost/assign/list_of.hpp>

#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

namespace visuVTKAdaptor
{

static ::fwServices::QtQmlRegistar<SQMedical3DCamera>   registar("::visuVTKAdaptor::SMedical3DCamera");

static const ::fwCom::Slots::SlotKeyType SET_AXIAL_SLOT    = "setAxial";
static const ::fwCom::Slots::SlotKeyType SET_SAGITTAL_SLOT = "setSagittal";
static const ::fwCom::Slots::SlotKeyType SET_FRONTAL_SLOT  = "setFrontal";

std::map< std::string, ::fwDataTools::helper::MedicalImageAdaptor::Orientation >
SQMedical3DCamera::m_orientationConversion = ::boost::assign::map_list_of
                                                 (std::string("axial"), Z_AXIS)
                                                 (std::string("frontal"), Y_AXIS)
                                                 (std::string("sagittal"), X_AXIS);

//------------------------------------------------------------------------------

SQMedical3DCamera::SQMedical3DCamera() noexcept :
    m_resetAtStart(false)

{
    newSlot(SET_AXIAL_SLOT, &SQMedical3DCamera::setAxialView, this);
    newSlot(SET_SAGITTAL_SLOT, &SQMedical3DCamera::setSagittalView, this);
    newSlot(SET_FRONTAL_SLOT, &SQMedical3DCamera::setFrontalView, this);
}

//------------------------------------------------------------------------------

SQMedical3DCamera::~SQMedical3DCamera() noexcept
{
}

//------------------------------------------------------------------------------

void SQMedical3DCamera::configuring()
{
    this->configureParams();
/*
    const ConfigType config = this->getConfigTree().get_child("config.<xmlattr>");

    const std::string orientation = config.get<std::string>("sliceIndex", "axial");
    SLM_ASSERT("Unknown orientation", m_orientationConversion.find(orientation) != m_orientationConversion.end());
    m_orientation = m_orientationConversion[orientation];

    const std::string reset = config.get<std::string>("resetAtStart", "no");
    SLM_ASSERT("'resetAtStart' value must be 'yes' or 'no'", reset == "yes" || reset == "no");
    m_resetAtStart = (reset == "yes");*/
}

//------------------------------------------------------------------------------

void SQMedical3DCamera::starting()
{
    this->initialize();

    m_camera = this->getRenderer()->GetActiveCamera();

    this->getRenderer()->ResetCamera();
    if(m_resetAtStart)
    {
        this->updateView();
    }
}

//------------------------------------------------------------------------------

void SQMedical3DCamera::updating()
{
    this->updateView();
    this->requestRender();
}

//------------------------------------------------------------------------------

void SQMedical3DCamera::stopping()
{
}

//------------------------------------------------------------------------------

void SQMedical3DCamera::setSagittalView()
{
    m_orientation = X_AXIS;
    this->updating();
}

//------------------------------------------------------------------------------

void SQMedical3DCamera::setFrontalView()
{
    m_orientation = Y_AXIS;
    this->updating();
}

//------------------------------------------------------------------------------

void SQMedical3DCamera::setAxialView()
{
    m_orientation = Z_AXIS;
    this->updating();
}

//------------------------------------------------------------------------------

void SQMedical3DCamera::updateView()
{
    if(m_orientation == Z_AXIS )
    {
        this->resetAxialView();
    }
    else if(m_orientation == Y_AXIS )
    {
        this->resetFrontalView();
    }
    else if(m_orientation == X_AXIS )
    {
        this->resetSagittalView();
    }
}

//------------------------------------------------------------------------------

void SQMedical3DCamera::resetSagittalView()
{
    m_camera->SetPosition(-1, 0, 0);
    m_camera->SetFocalPoint(0, 0, 0);
    m_camera->SetViewUp(0, 0, 1);
    this->getRenderer()->ResetCamera();
    this->setVtkPipelineModified();
}

//------------------------------------------------------------------------------

void SQMedical3DCamera::resetFrontalView()
{
    m_camera->SetPosition(0, -1, 0);
    m_camera->SetFocalPoint(0, 0, 0);
    m_camera->SetViewUp(0, 0, 1);
    this->getRenderer()->ResetCamera();
    this->setVtkPipelineModified();

}

//------------------------------------------------------------------------------

void SQMedical3DCamera::resetAxialView()
{
    m_camera->SetPosition(0, 0, -1);
    m_camera->SetFocalPoint(0, 0, 0);
    m_camera->SetViewUp(0, -1, 0);
    this->getRenderer()->ResetCamera();
    this->setVtkPipelineModified();
}

//------------------------------------------------------------------------------

} //namespace visuVTKAdaptor
