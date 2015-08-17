/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2014-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "videoCalibration/SChessBoardDetector.hpp"

#include <fwData/Composite.hpp>
#include <fwData/Array.hpp>

#include <arData/CalibrationInfo.hpp>

#include <fwCom/Signal.hxx>
#include <fwCom/Slots.hxx>

#include <fwComEd/helper/Array.hpp>

#include <fwServices/registry/ObjectService.hpp>
#include <fwServices/IService.hpp>
#include <fwServices/Base.hpp>

#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

namespace videoCalibration
{
fwServicesRegisterMacro(::fwServices::IController, ::videoCalibration::SChessBoardDetector, ::fwData::Composite);

const ::fwCom::Slots::SlotKeyType SChessBoardDetector::s_CHECK_POINTS_SLOT           = "checkPoints";
const ::fwCom::Slots::SlotKeyType SChessBoardDetector::s_DETECT_POINTS_SLOT          = "detectPoints";
const ::fwCom::Slots::SlotKeyType SChessBoardDetector::s_UPDATE_CHESSBOARD_SIZE_SLOT = "updateChessboardSize";

const ::fwCom::Signals::SignalKeyType SChessBoardDetector::s_CHESSBOARD_DETECTED_SIG     = "chessboardDetected";
const ::fwCom::Signals::SignalKeyType SChessBoardDetector::s_CHESSBOARD_NOT_DETECTED_SIG = "chessboardNotDetected";

// ----------------------------------------------------------------------------

SChessBoardDetector::SChessBoardDetector() throw () :
    m_width(0),
    m_height(0),
    m_isDetected(false),
    m_lastTimestamp(0)
{
    m_sigChessboardDetected    = newSignal< ChessboardDetectedSignalType >( s_CHESSBOARD_DETECTED_SIG );
    m_sigChessboardNotDetected = newSignal< ChessboardNotDetectedSignalType >( s_CHESSBOARD_NOT_DETECTED_SIG );

    newSlot( s_CHECK_POINTS_SLOT, &SChessBoardDetector::checkPoints, this );
    newSlot( s_DETECT_POINTS_SLOT, &SChessBoardDetector::detectPoints, this );
    newSlot( s_UPDATE_CHESSBOARD_SIZE_SLOT, &SChessBoardDetector::updateChessboardSize, this );
}

// ----------------------------------------------------------------------------

SChessBoardDetector::~SChessBoardDetector() throw ()
{
}

// ----------------------------------------------------------------------------

void SChessBoardDetector::configuring() throw (fwTools::Failed)
{
    ::fwRuntime::ConfigurationElementContainer caliCfgs = m_configuration->findAllConfigurationElement("calibration");
    SLM_ASSERT("Can not find calibration element", caliCfgs.size() >= 1 );
    typedef ::fwRuntime::ConfigurationElement::sptr ConfigurationType;

    for(ConfigurationType elem :  caliCfgs.getElements())
    {
        SLM_ASSERT("Attribute 'timeline' is missing.", elem->hasAttribute("timeline"));
        std::string tl = elem->getAttributeValue("timeline");
        SLM_ASSERT("Attribute 'timeline' is empty", !tl.empty());

        m_timeLineKeys.push_back(tl);

        SLM_ASSERT("Attribute 'calInfo' is missing.", elem->hasAttribute("calInfo"));
        std::string calInfo = elem->getAttributeValue("calInfo");
        SLM_ASSERT("Attribute 'calInfo' is empty", !calInfo.empty());

        m_calInfoKeys.push_back(calInfo);
        m_pointsLists.push_back(nullptr);
    }

    ::fwRuntime::ConfigurationElement::sptr cfgBoard = m_configuration->findConfigurationElement("board");
    SLM_ASSERT("Tag 'board' not found.", cfgBoard);

    SLM_ASSERT("Attribute 'width' is missing.", cfgBoard->hasAttribute("width"));
    std::string width = cfgBoard->getAttributeValue("width");
    SLM_ASSERT("Attribute 'width' is empty", !width.empty());
    m_width = ::boost::lexical_cast< size_t > (width);

    SLM_ASSERT("Attribute 'height' is missing.", cfgBoard->hasAttribute("height"));
    std::string height = cfgBoard->getAttributeValue("height");
    SLM_ASSERT("Attribute 'height' is empty", !height.empty());
    m_height = ::boost::lexical_cast< size_t > (height);
}

// ----------------------------------------------------------------------------

void SChessBoardDetector::starting() throw (fwTools::Failed)
{
    ::fwData::Composite::sptr composite = this->getObject< ::fwData::Composite >();

    // Grab timeline objects
    for(const std::string &tlKey : m_timeLineKeys)
    {
        ::fwData::Composite::IteratorType it = composite->find(tlKey);
        SLM_ASSERT("Missing '" + tlKey + "' key in the composite", it !=  composite->end());
        ::extData::FrameTL::sptr frameTL = ::extData::FrameTL::dynamicCast(it->second);
        SLM_ASSERT("'" + tlKey + "' key is not a ::extData::FrameTL", frameTL);

        m_frameTLs.push_back(frameTL);
    }
}

// ----------------------------------------------------------------------------

void SChessBoardDetector::swapping() throw (fwTools::Failed)
{
}

// ----------------------------------------------------------------------------

void SChessBoardDetector::updating() throw (fwTools::Failed)
{
}

// ----------------------------------------------------------------------------

void SChessBoardDetector::stopping() throw (fwTools::Failed)
{
}

// ----------------------------------------------------------------------------

void SChessBoardDetector::checkPoints( ::fwCore::HiResClock::HiResClockType timestamp)
{
    if (timestamp > m_lastTimestamp)
    {
        ::fwCore::HiResClock::HiResClockType lastTimestamp;
        lastTimestamp = std::numeric_limits< ::fwCore::HiResClock::HiResClockType >::max();

        for(const auto& frameTL : m_frameTLs)
        {
            lastTimestamp = std::min(lastTimestamp, frameTL->getNewerTimestamp());
        }

        size_t idx = 0;
        m_isDetected = true;

        for(const auto& tl : m_frameTLs)
        {
            ::fwData::PointList::sptr chessBoardPoints = this->detectChessboard(tl, lastTimestamp, m_width, m_height);

            if(!chessBoardPoints)
            {
                m_isDetected = false;
                break;

            }
            m_pointsLists[idx] = chessBoardPoints;

            ++idx;
        }

        if(m_isDetected)
        {
            m_sigChessboardDetected->asyncEmit();
        }
        else
        {
            m_sigChessboardNotDetected->asyncEmit();
        }

        m_lastTimestamp = lastTimestamp;
    }
}

// ----------------------------------------------------------------------------

void SChessBoardDetector::detectPoints()
{
    if(m_isDetected)
    {
        ::fwData::Composite::sptr composite = this->getObject< ::fwData::Composite >();

        size_t idx = 0;
        for(const auto& key : m_calInfoKeys)
        {
            ::fwData::Composite::IteratorType it = composite->find(key);
            SLM_ASSERT("Missing '" + key + "' key in the composite", it !=  composite->end());
            ::arData::CalibrationInfo::sptr calInfo = ::arData::CalibrationInfo::dynamicCast(it->second);

            ::fwData::Image::sptr image = this->createImage( m_frameTLs[idx], m_lastTimestamp);

            calInfo->addRecord(image, m_pointsLists[idx]);

            // Notify
            ::arData::CalibrationInfo::AddedRecordSignalType::sptr sig;
            sig = calInfo->signal< ::arData::CalibrationInfo::AddedRecordSignalType >
                      (::arData::CalibrationInfo::s_ADDED_RECORD_SIG);

            sig->asyncEmit();

            ++idx;
        }
    }
}

// ----------------------------------------------------------------------------

void SChessBoardDetector::updateChessboardSize(const int width, const int height)
{
    m_width  = static_cast<size_t>(width);
    m_height = static_cast<size_t>(height);
}

// ----------------------------------------------------------------------------

::fwData::Image::sptr SChessBoardDetector::createImage( ::extData::FrameTL::sptr tl,
                                                        ::fwCore::HiResClock::HiResClockType timestamp)
{
    ::fwData::Image::sptr image;

    const CSPTR(::extData::FrameTL::BufferType) buffer = tl->getClosestBuffer(timestamp);
    if (buffer)
    {
        image = ::fwData::Image::New();

        ::fwData::Image::SizeType size(3);
        size[0] = tl->getWidth();
        size[1] = tl->getHeight();
        size[2] = 1;
        const ::fwData::Image::SpacingType::value_type voxelSize = 1;
        image->allocate(size, tl->getType(), tl->getNumberOfComponents());
        ::fwData::Image::OriginType origin(3,0);

        image->setOrigin(origin);
        ::fwData::Image::SpacingType spacing(3,voxelSize);
        image->setSpacing(spacing);
        image->setWindowWidth(1);
        image->setWindowCenter(0);

        ::fwData::Array::sptr array = image->getDataArray();

        ::fwComEd::helper::Array arrayHelper(array);

        const ::boost::uint8_t*  frameBuff = &buffer->getElement(0);
        ::boost::uint8_t* index = arrayHelper.begin< ::boost::uint8_t >();

        std::copy( frameBuff, frameBuff+buffer->getSize(), index);
    }

    return image;
}

// ----------------------------------------------------------------------------

SPTR(::fwData::PointList) SChessBoardDetector::detectChessboard(::extData::FrameTL::sptr tl,
                                                                ::fwCore::HiResClock::HiResClockType timestamp,
                                                                size_t xDim, size_t yDim)
{
    ::fwData::PointList::sptr pointlist;

    const CSPTR(::extData::FrameTL::BufferType) buffer = tl->getClosestBuffer(timestamp);

    if(buffer)
    {
        int height = static_cast<int>(tl->getWidth());
        int width  = static_cast<int>(tl->getHeight());

        ::boost::uint8_t* frameBuff = const_cast< ::boost::uint8_t*>( &buffer->getElement(0) );

        cv::Mat img(width, height, CV_8UC4, frameBuff);
        cv::Mat grayImg;
        cv::cvtColor(img, grayImg, CV_RGBA2GRAY);
        cv::Size boardSize(static_cast<int>(xDim) - 1, static_cast<int>(yDim) - 1);
        std::vector< cv::Point2f > corners;

        int flags = CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE | CV_CALIB_CB_FILTER_QUADS
                    | CV_CALIB_CB_FAST_CHECK;

        if (cv::findChessboardCorners(grayImg, boardSize, corners, flags))
        {
            cv::TermCriteria term(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 30, 0.1);
            cv::cornerSubPix(grayImg, corners, cv::Size(5, 5), cv::Size(-1, -1), term);

            pointlist                                       = ::fwData::PointList::New();
            ::fwData::PointList::PointListContainer &points = pointlist->getRefPoints();
            points.reserve(corners.size());

            for(cv::Point2f& p : corners)
            {
                ::fwData::Point::sptr point = ::fwData::Point::New(p.x, p.y);
                points.push_back(point);
            }
        }
    }

    return pointlist;
}

// ----------------------------------------------------------------------------

} //namespace videoCalibration

