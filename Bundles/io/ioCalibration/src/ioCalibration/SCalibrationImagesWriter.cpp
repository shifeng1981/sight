/************************************************************************
 *
 * Copyright (C) 2019 IRCAD France
 * Copyright (C) 2019 IHU Strasbourg
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

#include "ioCalibration/SCalibrationImagesWriter.hpp"

#include <arData/CalibrationInfo.hpp>

#include <cvIO/Image.hpp>

#include <fwData/Image.hpp>
#include <fwData/location/Folder.hpp>

#include <fwGui/Cursor.hpp>
#include <fwGui/dialog/LocationDialog.hpp>
#include <fwGui/dialog/MessageDialog.hpp>

#include <fwServices/macros.hpp>

#include <boost/filesystem/path.hpp>

#include <opencv2/opencv.hpp>

namespace ioCalibration
{

//------------------------------------------------------------------------------

SCalibrationImagesWriter::SCalibrationImagesWriter() noexcept
{
}

//------------------------------------------------------------------------------

SCalibrationImagesWriter::~SCalibrationImagesWriter() noexcept
{

}

//------------------------------------------------------------------------------

::fwIO::IOPathType SCalibrationImagesWriter::getIOPathType() const
{
    return ::fwIO::FOLDER;
}

//------------------------------------------------------------------------------

void SCalibrationImagesWriter::configureWithIHM()
{
    static ::boost::filesystem::path s_defaultPath;

    ::fwGui::dialog::LocationDialog dialogFile;
    dialogFile.setTitle(m_windowTitle.empty() ? "Choose a folder to save the images" : m_windowTitle);
    dialogFile.setDefaultLocation( ::fwData::location::Folder::New(s_defaultPath) );
    dialogFile.setOption(::fwGui::dialog::ILocationDialog::WRITE);
    dialogFile.setType(::fwGui::dialog::ILocationDialog::FOLDER);

    ::fwData::location::Folder::sptr result = ::fwData::location::Folder::dynamicCast(dialogFile.show());

    if (result)
    {
        s_defaultPath = result->getFolder().parent_path();
        dialogFile.saveDefaultLocation( ::fwData::location::Folder::New(s_defaultPath) );
        this->setFolder(result->getFolder());
    }
    else
    {
        this->clearLocations();
    }
}

//------------------------------------------------------------------------------

void SCalibrationImagesWriter::configuring()
{
    ::fwIO::IWriter::configuring();

    const auto configTree = this->getConfigTree();
    m_fileExtension = configTree.get("format", ".tiff");
}

//------------------------------------------------------------------------------

void SCalibrationImagesWriter::starting()
{
}

//------------------------------------------------------------------------------

void SCalibrationImagesWriter::updating()
{
    if( !m_fileExtension.empty() && this->hasLocationDefined() )
    {
        ::arData::CalibrationInfo::sptr calibInfo =
            this->getInOut< ::arData::CalibrationInfo >(::fwIO::s_DATA_KEY);

        ::fwGui::Cursor cursor;
        cursor.setCursor(::fwGui::ICursor::BUSY);

        size_t count(0);
        for(auto& calibImg : calibInfo->getImageContainer())
        {
            const std::string filename           = "img_" + std::to_string(count++) + m_fileExtension;
            const ::boost::filesystem::path path = this->getFolder() / filename;

            const ::cv::Mat cvImg = ::cvIO::Image::moveToCv(calibImg);
            // Make sure we're handling a 'real' (depth-less) 2D image.
            const ::cv::Mat img2D = cvImg.reshape(0, 2, cvImg.size);

            try
            {
                if (img2D.type() == CV_8UC3 || img2D.type() == CV_8UC4)
                {
                    // convert the read image from BGR to RGB
                    ::cv::Mat imgRgb;
                    const auto colConvType = img2D.type() == CV_8UC3 ? ::cv::COLOR_BGR2RGB : ::cv::COLOR_BGRA2RGBA;
                    ::cv::cvtColor(img2D, imgRgb, colConvType);
                    ::cv::imwrite(path.string(), imgRgb);
                }
                else
                {
                    ::cv::imwrite(path.string(), img2D);
                }
            }
            catch(const ::cv::Exception& e)
            {
                ::fwGui::dialog::MessageDialog::showMessageDialog("Error writing calibration images.",
                                                                  e.what(),
                                                                  ::fwGui::dialog::MessageDialog::CRITICAL);
            }
        }

        cursor.setDefaultCursor();
    }
}

//------------------------------------------------------------------------------

void SCalibrationImagesWriter::stopping()
{
}

//------------------------------------------------------------------------------

} // namespace ioCalibration
