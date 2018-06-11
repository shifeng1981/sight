/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "videoCharucoCalibration/SOpenCVIntrinsic.hpp"

#include <arData/CalibrationInfo.hpp>
#include <arData/Camera.hpp>

#include <calibration3d/helper.hpp>

#include <fwCom/Signal.hxx>
#include <fwCom/Slots.hxx>

#include <fwData/mt/ObjectReadLock.hpp>
#include <fwData/mt/ObjectWriteLock.hpp>
#include <fwData/PointList.hpp>
#include <fwData/TransformationMatrix3D.hpp>
#include <fwData/Vector.hpp>

#include <fwDataTools/TransformationMatrix3D.hpp>

#include <fwPreferences/helper.hpp>

#include <fwRuntime/ConfigurationElement.hpp>

#include <fwServices/IService.hpp>
#include <fwServices/macros.hpp>

#include <fwTools/fwID.hpp>
#include <fwTools/Object.hpp>

#include <opencv2/aruco.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

fwServicesRegisterMacro(::arServices::ICalibration, ::videoCharucoCalibration::SOpenCVIntrinsic, ::arData::Camera);

namespace videoCharucoCalibration
{

static const ::fwCom::Slots::SlotKeyType s_UPDATE_CHARUCOBOARD_SIZE_SLOT = "updateCharucoBoardSize";

static const ::fwCom::Signals::SignalKeyType s_ERROR_COMPUTED_SIG = "errorComputed";

// ----------------------------------------------------------------------------

SOpenCVIntrinsic::SOpenCVIntrinsic() noexcept :
    m_width(11),
    m_height(8),
    m_squareSize(20.0)
{
    newSignal<ErrorComputedSignalType>(s_ERROR_COMPUTED_SIG);
    newSlot(s_UPDATE_CHARUCOBOARD_SIZE_SLOT, &SOpenCVIntrinsic::updateCharucoBoardSize, this);
}

// ----------------------------------------------------------------------------

SOpenCVIntrinsic::~SOpenCVIntrinsic() noexcept
{
}

//------------------------------------------------------------------------------

void SOpenCVIntrinsic::configuring()
{
    const auto configTree = this->getConfigTree();
    const auto cfgBoard   = configTree.get_child("board.<xmlattr>");

    m_widthKey            = cfgBoard.get<std::string>("width", "CHESSBOARD_WIDTH");
    m_heightKey           = cfgBoard.get<std::string>("height", "CHESSBOARD_HEIGHT");
    m_squareSizeKey       = cfgBoard.get<std::string>("squareSize", "CHESSBOARD_SQUARE_SIZE");
    m_markerSizeKey       = cfgBoard.get<std::string>("markerSize", "CHESSBOARD_MARKER_SIZE");
    m_markerSizeInBitsKey = cfgBoard.get<std::string>("markerSizeInBits", "CHESSBOARD_MARKER_SIZE_IN_BITS");

    this->updateCharucoBoardSize();
}

// ----------------------------------------------------------------------------

void SOpenCVIntrinsic::starting()
{
    this->updateCharucoBoardSize();
}

// ----------------------------------------------------------------------------

void SOpenCVIntrinsic::stopping()
{
}

//------------------------------------------------------------------------------

void SOpenCVIntrinsic::swapping()
{
    this->stopping();
    this->starting();
}

//--------------------------------------------------------------------- ---------

void SOpenCVIntrinsic::updating()
{
    ::arData::CalibrationInfo::csptr calInfo = this->getInput< ::arData::CalibrationInfo>("calibrationInfo");
    ::arData::Camera::sptr cam               = this->getInOut< ::arData::Camera >("camera");
    ::fwData::Vector::sptr poseCamera        = this->getInOut< ::fwData::Vector >("poseVector");

    SLM_ASSERT("Object with 'calibrationInfo' is not found", calInfo);
    SLM_WARN_IF("Calibration info is empty.", calInfo->getPointListContainer().empty());

    if(!calInfo->getPointListContainer().empty())
    {
        std::vector<std::vector< ::cv::Point2f > > cornersPoints;
        std::vector<std::vector< int > > ids;

        {
            ::fwData::mt::ObjectReadLock calInfoLock(calInfo);
            for(::fwData::PointList::sptr capture : calInfo->getPointListContainer())
            {
                std::vector< ::cv::Point2f > cdst;
                std::vector< int > idst;

                for(::fwData::Point::csptr point : capture->getPoints())
                {
                    SLM_ASSERT("point is null", point);
                    cdst.push_back(::cv::Point2f(static_cast<float>(point->getCoord()[0]),
                                                 static_cast<float>(point->getCoord()[1])));
                    idst.push_back(static_cast<int>(point->getCoord()[2]));
                }
                cornersPoints.push_back(cdst);
                ids.push_back(idst);

            }
        }

        ::fwData::Image::sptr img = calInfo->getImageContainer().front();

        ::cv::Mat cameraMatrix;
        std::vector<float> distCoeffs;
        std::vector<cv::Mat> rvecs;
        std::vector<cv::Mat> tvecs;
        ::cv::Size2i imgsize(static_cast<int>(img->getSize()[0]), static_cast<int>(img->getSize()[1]));

        double err = ::cv::aruco::calibrateCameraCharuco(cornersPoints, ids, m_board, imgsize, cameraMatrix, distCoeffs,
                                                         rvecs, tvecs);

        this->signal<ErrorComputedSignalType>(s_ERROR_COMPUTED_SIG)->asyncEmit(err);

        if(poseCamera)
        {
            poseCamera->getContainer().clear();

            for(size_t index = 0; index < rvecs.size(); ++index)
            {
                ::fwData::TransformationMatrix3D::sptr mat3D = ::fwData::TransformationMatrix3D::New();

                ::cv::Mat rmat;
                ::cv::Rodrigues(rvecs.at(index), rmat);

                ::cv::Mat tmat = tvecs.at(index);

                for(size_t i = 0; i < 3; ++i)
                {
                    for(size_t j = 0; j < 3; ++j)
                    {
                        mat3D->setCoefficient(i, j, rmat.at< double >(static_cast<int>(i),
                                                                      static_cast<int>(j)));
                    }
                    mat3D->setCoefficient(i, 3, tmat.at< double >(static_cast<int>(i)));
                }
                //::fwDataTools::TransformationMatrix3D::invert(mat3D, mat3D);
                poseCamera->getContainer().push_back(mat3D);
                auto sig = poseCamera->signal< ::fwData::Vector::AddedObjectsSignalType >(
                    ::fwData::Vector::s_ADDED_OBJECTS_SIG);
                sig->asyncEmit(poseCamera->getContainer());
            }
        }

        OSLM_DEBUG("Calibration error :" << err);

        ::fwData::mt::ObjectWriteLock camLock(cam);

        cam->setCx(cameraMatrix.at<double>(0, 2));
        cam->setCy(cameraMatrix.at<double>(1, 2));
        cam->setFx(cameraMatrix.at<double>(0, 0));
        cam->setFy(cameraMatrix.at<double>(1, 1));
        cam->setWidth(img->getSize()[0]);
        cam->setHeight(img->getSize()[1]);
        cam->setDistortionCoefficient(distCoeffs[0], distCoeffs[1], distCoeffs[2], distCoeffs[3], distCoeffs[4]);

        cam->setIsCalibrated(true);

        ::arData::Camera::IntrinsicCalibratedSignalType::sptr sig;
        sig = cam->signal< ::arData::Camera::IntrinsicCalibratedSignalType >(
            ::arData::Camera::s_INTRINSIC_CALIBRATED_SIG);

        sig->asyncEmit();
    }
}

//------------------------------------------------------------------------------

void SOpenCVIntrinsic::updateCharucoBoardSize()
{
    const std::string widthStr = ::fwPreferences::getPreference(m_widthKey);
    if(!widthStr.empty())
    {
        m_width = std::stoul(widthStr);
    }
    const std::string heightStr = ::fwPreferences::getPreference(m_heightKey);
    if(!heightStr.empty())
    {
        m_height = std::stoul(heightStr);
    }
    const std::string squareSizeStr = ::fwPreferences::getPreference(m_squareSizeKey);
    if(!squareSizeStr.empty())
    {
        m_squareSize = std::stof(squareSizeStr);
    }
    const std::string markerSizeStr = ::fwPreferences::getPreference(m_markerSizeKey);
    if(!markerSizeStr.empty())
    {
        m_markerSize = std::stof(markerSizeStr);
    }
    const std::string markerSizeInBitsStr = ::fwPreferences::getPreference(m_markerSizeInBitsKey);
    if(!markerSizeInBitsStr.empty())
    {
        m_markerSizeInBits = std::stoi(markerSizeInBitsStr);
    }

    m_dictionary = ::calibration3d::helper::generateArucoDictionary(m_width, m_height, m_markerSizeInBits);

    ::cv::Size boardSize(static_cast<int>(m_width), static_cast<int>(m_height));

    m_board = ::cv::aruco::CharucoBoard::create(boardSize.width, boardSize.height, m_squareSize,
                                                m_markerSize, m_dictionary);
}

//------------------------------------------------------------------------------

} // namespace videoCharucoCalibration
