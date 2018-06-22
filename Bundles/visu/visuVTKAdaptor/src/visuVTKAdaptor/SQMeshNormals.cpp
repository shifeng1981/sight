/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "visuVTKAdaptor/SQMeshNormals.hpp"

#include <fwCom/Slot.hpp>
#include <fwCom/Slot.hxx>
#include <fwCom/Slots.hpp>
#include <fwCom/Slots.hxx>

#include <fwData/Mesh.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/QtQmlType.hxx>

#include <fwVtkIO/helper/Mesh.hpp>

#include <boost/assign/list_of.hpp>

#include <vtkActor.h>
#include <vtkArrowSource.h>
#include <vtkCellCenters.h>
#include <vtkGlyph3D.h>
#include <vtkGlyphSource2D.h>
#include <vtkMaskPoints.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

namespace visuVTKAdaptor
{

static ::fwServices::QtQmlType<SQMeshNormals>   registar("com.fw4spl.vtk.adaptors", 1, 0, "SMeshNormals");

const ::fwServices::IService::KeyType SQMeshNormals::s_MESH_INPUT = "mesh";

//------------------------------------------------------------------------------

std::map< std::string, SQMeshNormals::NormalRepresentation >
SQMeshNormals::m_normalRepresentationConversion
    = ::boost::assign::map_list_of(std::string("POINT"), POINT_NORMAL)
          (std::string("CELL"), CELL_NORMAL)
          (std::string("NONE"), NONE);

//------------------------------------------------------------------------------

const ::fwCom::Slots::SlotKeyType SQMeshNormals::s_UPDATE_VERTEX_SLOT        = "updateVertex";
const ::fwCom::Slots::SlotKeyType SQMeshNormals::s_UPDATE_POINT_NORMALS_SLOT = "updatePointNormals";
const ::fwCom::Slots::SlotKeyType SQMeshNormals::s_UPDATE_CELL_NORMALS_SLOT  = "updateCellNormals";
const ::fwCom::Slots::SlotKeyType SQMeshNormals::s_SHOW_POINT_NORMALS_SLOT   = "showPointNormals";
const ::fwCom::Slots::SlotKeyType SQMeshNormals::s_SHOW_CELL_NORMALS_SLOT    = "showCellNormals";
const ::fwCom::Slots::SlotKeyType SQMeshNormals::s_HIDE_NORMALS_SLOT         = "hideNormals";
const ::fwCom::Slots::SlotKeyType SQMeshNormals::s_UPDATE_NORMAL_MODE_SLOT   = "updateNormalMode";

//------------------------------------------------------------------------------

SQMeshNormals::SQMeshNormals() noexcept :
    m_normalRepresentation(CELL_NORMAL)
{
    m_actor = vtkActor::New();

    newSlot(s_UPDATE_VERTEX_SLOT, &SQMeshNormals::updateVertex, this);
    newSlot(s_UPDATE_POINT_NORMALS_SLOT, &SQMeshNormals::updatePointNormals, this);
    newSlot(s_UPDATE_CELL_NORMALS_SLOT, &SQMeshNormals::updateCellNormals, this);
    newSlot(s_SHOW_POINT_NORMALS_SLOT, &SQMeshNormals::showPointNormals, this);
    newSlot(s_SHOW_CELL_NORMALS_SLOT, &SQMeshNormals::showCellNormals, this);
    newSlot(s_HIDE_NORMALS_SLOT, &SQMeshNormals::hideNormals, this);
    newSlot(s_UPDATE_NORMAL_MODE_SLOT, &SQMeshNormals::updateNormalMode, this);
}

//------------------------------------------------------------------------------

SQMeshNormals::~SQMeshNormals() noexcept
{
    m_actor->Delete();
    m_actor = 0;
}

//------------------------------------------------------------------------------

void SQMeshNormals::configuring()
{
    this->configureParams();

    /* const ConfigType config = this->getConfigTree().get_child("config.<xmlattr>");

       if(config.count("normal") )
       {
         const std::string normal = config.get<std::string>("normal");
         SLM_ASSERT("Wrong normal representation '" + normal + "' (required POINT, CELL or NONE)",
                    m_normalRepresentationConversion.find(normal) != m_normalRepresentationConversion.end());

         m_normalRepresentation = m_normalRepresentationConversion[normal];
       }*/
}

//------------------------------------------------------------------------------

void SQMeshNormals::starting()
{
    this->initialize();
    this->updating();
    this->addToRenderer(this->getActor());
    this->requestRender();
}

//------------------------------------------------------------------------------

void SQMeshNormals::updating()
{
    this->updateMeshNormals();
    this->requestRender();
}

//------------------------------------------------------------------------------

vtkActor* SQMeshNormals::getActor() const
{
    return m_actor;
}

//------------------------------------------------------------------------------

void SQMeshNormals::setPolyData(vtkSmartPointer< vtkPolyData > polydata)
{
    if (polydata)
    {
        m_polyData = polydata;
    }
}

//------------------------------------------------------------------------------

void SQMeshNormals::updateMeshNormals()
{
    ::fwData::Mesh::csptr mesh = this->getInOut< ::fwData::Mesh >(s_MESH_INPUT);
    SLM_ASSERT("Missing mesh", mesh);

    if(m_normalRepresentation == NONE)
    {
        m_actor->SetVisibility( false );
    }
    else
    {
        m_polyData = vtkSmartPointer< vtkPolyData >::New();
        ::fwVtkIO::helper::Mesh::toVTKMesh(mesh, m_polyData);

        vtkSmartPointer<vtkPolyDataAlgorithm> algo;
        if(m_normalRepresentation == CELL_NORMAL)
        {
            algo = vtkSmartPointer<vtkCellCenters>::New();
        }
        else if(m_normalRepresentation == POINT_NORMAL)
        {
            vtkSmartPointer<vtkMaskPoints> ptMask = vtkSmartPointer<vtkMaskPoints>::New();
            ptMask->SetOnRatio(1);
            ptMask->RandomModeOn();
            ptMask->SetMaximumNumberOfPoints(static_cast<vtkIdType>(mesh->getNumberOfPoints()));
            algo = ptMask;
        }

        algo->SetInputData(m_polyData);

        vtkSmartPointer<vtkGlyphSource2D> arrow = vtkSmartPointer<vtkGlyphSource2D>::New();
        arrow->SetGlyphTypeToArrow();
        arrow->FilledOff();

        vtkSmartPointer<vtkGlyph3D> glyph = vtkSmartPointer<vtkGlyph3D>::New();
        glyph->SetInputConnection(algo->GetOutputPort());
        glyph->SetSourceConnection(arrow->GetOutputPort());
        glyph->SetVectorModeToUseNormal();
        glyph->SetScaleModeToScaleByVector();
        glyph->SetScaleFactor(10.0);

        vtkSmartPointer<vtkPolyDataMapper> glyphMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        glyphMapper->SetInputConnection(glyph->GetOutputPort());

        m_actor->SetVisibility( true );
        this->getActor()->SetMapper(glyphMapper);
        this->getActor()->GetProperty()->SetColor(1., 1., 1.);
    }
    this->setVtkPipelineModified();
    this->requestRender();
}

//------------------------------------------------------------------------------

void SQMeshNormals::stopping()
{
    this->removeAllPropFromRenderer();
    this->requestRender();
}

//------------------------------------------------------------------------------

void SQMeshNormals::updateVertex()
{
    ::fwData::Mesh::sptr mesh = this->getInOut < ::fwData::Mesh >(s_MESH_INPUT);
    ::fwVtkIO::helper::Mesh::updatePolyDataPoints(m_polyData, mesh);
    ::fwVtkIO::helper::Mesh::updatePolyDataPointNormals(m_polyData, mesh);
    ::fwVtkIO::helper::Mesh::updatePolyDataPointNormals(m_polyData, mesh);
    this->setVtkPipelineModified();
    this->requestRender();
}

//------------------------------------------------------------------------------

void SQMeshNormals::updatePointNormals()
{
    ::fwData::Mesh::sptr mesh = this->getInOut < ::fwData::Mesh >(s_MESH_INPUT);
    ::fwVtkIO::helper::Mesh::updatePolyDataPointNormals(m_polyData, mesh);
    this->setVtkPipelineModified();
    this->requestRender();
}

//------------------------------------------------------------------------------

void SQMeshNormals::updateCellNormals()
{
    ::fwData::Mesh::sptr mesh = this->getInOut < ::fwData::Mesh >(s_MESH_INPUT);
    ::fwVtkIO::helper::Mesh::updatePolyDataCellNormals(m_polyData, mesh);
    this->setVtkPipelineModified();
    this->requestRender();
}

//------------------------------------------------------------------------------

void SQMeshNormals::showPointNormals()
{
    m_normalRepresentation = POINT_NORMAL;
    this->updateMeshNormals();
}

//------------------------------------------------------------------------------

void SQMeshNormals::showCellNormals()
{
    m_normalRepresentation = CELL_NORMAL;
    this->updateMeshNormals();
}

//------------------------------------------------------------------------------

void SQMeshNormals::hideNormals()
{
    m_normalRepresentation = NONE;
    this->updateMeshNormals();
}

//------------------------------------------------------------------------------

void SQMeshNormals::updateNormalMode(std::uint8_t mode)
{
    switch (mode)
    {
        case 0:
        {
            this->hideNormals();
            break;
        }
        case 1:
        {
            this->showPointNormals();
            break;
        }
        case 2:
        {
            this->showCellNormals();
            break;
        }
        default:
        {
            OSLM_ERROR("mode " << mode << " is not allowed");
            break;
        }
    }
}

//------------------------------------------------------------------------------

} //namespace visuVTKAdaptor
