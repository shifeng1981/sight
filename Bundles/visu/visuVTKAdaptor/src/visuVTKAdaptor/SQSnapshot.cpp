/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "visuVTKAdaptor/SQSnapshot.hpp"

#include <fwCom/Slot.hpp>
#include <fwCom/Slot.hxx>
#include <fwCom/Slots.hpp>
#include <fwCom/Slots.hxx>

#include <fwData/Composite.hpp>
#include <fwData/String.hpp>

#include <fwDataTools/fieldHelper/Image.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/QtQmlType.hxx>

#include <fwVtkIO/vtk.hpp>

#include <boost/filesystem.hpp>

#include <vtkActor.h>
#include <vtkBMPWriter.h>
#include <vtkImageData.h>
#include <vtkImageWriter.h>
#include <vtkJPEGWriter.h>
#include <vtkPNGWriter.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkTIFFWriter.h>
#include <vtkWindowToImageFilter.h>

namespace visuVTKAdaptor
{

static ::fwServices::QtQmlType<SQSnapshot>   registar("com.fw4spl.vtk.adaptors", 1, 0, "SSnapshot");

const ::fwCom::Slots::SlotKeyType SQSnapshot::s_SNAP_SLOT        = "snap";
const ::fwCom::Slots::SlotKeyType SQSnapshot::s_SNAPTOIMAGE_SLOT = "snapToImage";

SQSnapshot::SQSnapshot() noexcept
{
    newSlot(s_SNAP_SLOT, &SQSnapshot::snap, this);
    newSlot(s_SNAPTOIMAGE_SLOT, &SQSnapshot::snapToImage, this);
}

//------------------------------------------------------------------------------

SQSnapshot::~SQSnapshot() noexcept
{
}

//------------------------------------------------------------------------------

void SQSnapshot::configuring()
{
    this->configureParams();
}

//------------------------------------------------------------------------------

void SQSnapshot::starting()
{
}

//------------------------------------------------------------------------------

void SQSnapshot::updating()
{
}

//------------------------------------------------------------------------------

void SQSnapshot::stopping()
{
}

//------------------------------------------------------------------------------

void SQSnapshot::snapToImage()
{

}

//------------------------------------------------------------------------------

void SQSnapshot::snap(QString const& _filePath)
{
    std::string filePath = _filePath.toStdString();
    SLM_ASSERT("filePath is empty", !filePath.empty());
    namespace fs = ::boost::filesystem;
    fs::path pathImageSnap(filePath);

    std::string ext = ".jpg";
    ext = pathImageSnap.extension().string();
    vtkImageWriter* writer = 0;

    if( ext == ".jpg" || ext == ".jpeg" )
    {
        writer = vtkJPEGWriter::New();
    }
    else if ( ext == ".bmp" )
    {
        writer = vtkBMPWriter::New();
    }
    else if ( ext == ".tiff" )
    {
        writer = vtkTIFFWriter::New();
    }
    else if ( ext == ".png" )
    {
        writer = vtkPNGWriter::New();
    }
    else
    {
        SLM_FATAL("Error: Format is not supported.");
    }

    vtkWindowToImageFilter* snapper = vtkWindowToImageFilter::New();
    snapper->SetMagnification( 1 );
    snapper->SetInput( this->getRenderer()->GetRenderWindow() );

    writer->SetInputConnection( snapper->GetOutputPort() );
    writer->SetFileName( pathImageSnap.string().c_str() );
    writer->Write();

    snapper->Delete();
    writer->Delete();
}

} //namespace visuVTKAdaptor
