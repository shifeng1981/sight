/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "uiVisuQt/QmlSnapshotEditor.hpp"

#include <fwCom/Signal.hpp>
#include <fwCom/Signal.hxx>
#include <fwCom/Signals.hpp>

#include <fwCore/base.hpp>

#include <fwData/location/Folder.hpp>
#include <fwData/location/SingleFile.hpp>

#include <fwGui/dialog/LocationDialog.hpp>
#include <fwGui/dialog/MessageDialog.hpp>

#include <fwGuiQt/container/QtContainer.hpp>

#include <fwRuntime/operations.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/QtQmlType.hxx>

#include <boost/filesystem.hpp>
#include <boost/filesystem/convenience.hpp>

#include <QHBoxLayout>
#include <QIcon>
#include <QPushButton>
#include <QString>
#include <QWidget>

namespace uiVisuQt
{

static ::fwServices::QtQmlType<QmlSnapshotEditor>   registar("com.fw4spl.uiVisuQt", 1, 0, "SnapshotEditor");

//------------------------------------------------------------------------------

QmlSnapshotEditor::QmlSnapshotEditor() noexcept
{
}

//------------------------------------------------------------------------------

QmlSnapshotEditor::~QmlSnapshotEditor() noexcept
{
}

//------------------------------------------------------------------------------

void QmlSnapshotEditor::starting()
{
}

//------------------------------------------------------------------------------

void QmlSnapshotEditor::stopping()
{
}

//------------------------------------------------------------------------------

void QmlSnapshotEditor::configuring()
{
}

//------------------------------------------------------------------------------

void QmlSnapshotEditor::updating()
{
}

//------------------------------------------------------------------------------

void QmlSnapshotEditor::onSnapButton()
{
    SLM_TRACE_FUNC();
    std::string filename = this->requestFileName();

    if(!filename.empty())
    {
        snapped(QString(filename.c_str()));
    }

}

//------------------------------------------------------------------------------

std::string QmlSnapshotEditor::requestFileName()
{
    std::string fileName = "";

    ::fwGui::dialog::LocationDialog dialogFile;
    dialogFile.setTitle("Save snapshot as");
    dialogFile.addFilter("Image file", "*.jpg *.jpeg *.bmp *.png *.tiff");
    dialogFile.addFilter("jpeg", "*.jpg *.jpeg");
    dialogFile.addFilter("bmp", "*.bmp");
    dialogFile.addFilter("png", "*.png");
    dialogFile.addFilter("tiff", "*.tiff");
    dialogFile.addFilter("all", "*.*");
    dialogFile.setOption(::fwGui::dialog::ILocationDialog::WRITE);

    ::fwData::location::SingleFile::sptr result;
    result = ::fwData::location::SingleFile::dynamicCast( dialogFile.show() );
    if (result)
    {
        fileName = result->getPath().string();
        dialogFile.saveDefaultLocation( ::fwData::location::Folder::New(result->getPath().parent_path()) );
    }

    return fileName;
}
//------------------------------------------------------------------------------

} // namespace uiVisuQt
