/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "videoOrbbec/SScan.hpp"

#include "videoOrbbec/ColorFrameWorker.hpp"

#include <fwCom/Signal.hxx>
#include <fwCom/Slots.hxx>

#include <fwData/Composite.hpp>

#include <fwGui/dialog/MessageDialog.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/registry/ActiveWorkers.hpp>

#include <fwVideoQt/helper/formats.hpp>
#include <fwVideoQt/Registry.hpp>

#include <QCameraInfo>

#include <memory>
#include <thread>

namespace videoOrbbec
{

//------------------------------------------------------------------------------

SScan::SScan() noexcept :
    QObject(),
    SScanBase(),
    m_depthTL(),
    m_colorTL(),
    m_rgbCamera(nullptr),
    m_oniStatus(),
    m_depthStream(),
    m_workerDepth(::fwThread::Worker::New()),
    m_pause(false),
    m_slotPresentDepthFrame(::fwCom::newSlot( &SScan::presentDepthFrame, this ))
{
    // Create a worker to fill the timelines
    m_slotPresentDepthFrame->setWorker(m_workerDepth);
}

//------------------------------------------------------------------------------

SScan::~SScan() noexcept
{
    this->stopCamera();
}

//------------------------------------------------------------------------------

// To start the service
void SScan::starting()
{
    m_colorTL = this->getInOut< ::arData::FrameTL>(s_FRAMETL_INOUT);
    m_depthTL = this->getInOut< ::arData::FrameTL>(s_DEPTHTL_INOUT);
}

//------------------------------------------------------------------------------

void SScan::startCamera()
{
    if(this->getWorker() != ::fwServices::registry::ActiveWorkers::getDefaultWorker())
    {
        const auto errMsg = "Astra grabbers currently don't support being run on a worker other than the main thread.";
        errorPopup(errMsg);
        FW_RAISE(errMsg);
    }
    try
    {
        this->stopCamera();

        if(!isReady())
        {
            tryInitialize();
        }

        // Initialize OpenNI and OpenCV video streams
        m_oniStatus = m_depthStream.create(*m_oniDevice, ::openni::SENSOR_DEPTH);
        FW_RAISE_IF("Cannot create depth stream", m_oniStatus != ::openni::STATUS_OK);
        m_oniStatus = m_depthStream.start();
        FW_RAISE_IF("Cannot open depth stream", m_oniStatus != ::openni::STATUS_OK);
        m_depthStream.setMirroringEnabled(false);

        // Initialize timelines
        const auto depthWidth  = m_depthStream.getVideoMode().getResolutionX(),
                   depthHeight = m_depthStream.getVideoMode().getResolutionY();

        const auto colorWidth  = m_rgbCamera->getWidth(),
                   colorHeight = m_rgbCamera->getHeight();

        m_depthTL->initPoolSize(static_cast<size_t>(depthWidth), static_cast<size_t>(depthHeight),
                                fwTools::Type::s_UINT16, 1);
        m_depthTL->setMaximumSize(50);

        m_colorTL->initPoolSize(colorWidth, colorHeight, fwTools::Type::s_UINT8, 4);
        m_colorTL->setMaximumSize(50);

#ifdef __linux__
        // open device with id
        m_rgbGrabber.open(m_rgbCamera->getCameraID());
#else
        // open device with index
        m_rgbGrabber.open(m_rgbCamera->getIndex());
#endif
        if(!m_rgbGrabber.isOpened())
        {
            throw std::runtime_error("cannot open camera: " + m_rgbCamera->getCameraID() + ".");
        }
        else
        {
            m_rgbGrabber.set(::cv::CAP_PROP_FPS, 30);
            m_rgbGrabber.set(::cv::CAP_PROP_FRAME_WIDTH, static_cast<double>(colorWidth));
            m_rgbGrabber.set(::cv::CAP_PROP_FRAME_HEIGHT, static_cast<double>(colorHeight));

        }

        m_slotPresentDepthFrame->asyncRun();

        auto sig = this->signal< ::arServices::IGrabber::CameraStartedSignalType>(
            ::arServices::IGrabber::s_CAMERA_STARTED_SIG);
        sig->asyncEmit();
    }
    catch(std::exception const& e)
    {
        errorPopup(std::string("Cannot start camera: ") + e.what());
        throw;
    }
}

//------------------------------------------------------------------------------

void SScan::tryInitialize()
{
    try
    {
        auto cameraInput = this->getInput< ::fwData::Object >(s_CAMERA_INPUT);
        auto camera      = ::arData::Camera::dynamicConstCast(cameraInput);

        if(camera)
        {
            m_rgbCamera = camera;
        }
        else
        {
            auto cameraSeries = ::arData::CameraSeries::dynamicConstCast(cameraInput);
            if(cameraSeries)
            {
                size_t numCamerasInSeries = cameraSeries->getNumberOfCameras();
                SLM_ASSERT("Camera Series is empty", numCamerasInSeries);

                // Assume same source on all cameras
                m_rgbCamera = cameraSeries->getCamera(0);
            }
        }

        FW_RAISE_IF("OpenNI initialization failed: " << ::openni::OpenNI::getExtendedError(),
                    ::openni::OpenNI::initialize() != ::openni::STATUS_OK);
        // Find and initialize the OpenNI device
        detectCameraOpenni();
        FW_RAISE_IF("Astra Pro camera not detected", !m_oniDevice || !m_oniDevice->isValid());

    }
    catch(std::exception const& e)
    {
        errorPopup(std::string("SScan initialization error: ") + e.what());
        throw;
    }
}

//------------------------------------------------------------------------------

bool SScan::isReady() const noexcept
{
    return m_oniDevice && m_rgbCamera;
}

//------------------------------------------------------------------------------

void SScan::stopCamera()
{
    m_depthStream.stop();
    m_rgbGrabber.release();

    this->signal< ::arServices::IGrabber::CameraStoppedSignalType>(::arServices::IGrabber::s_CAMERA_STOPPED_SIG)
    ->asyncEmit();

    m_pause = false;
}

// -----------------------------------------------------------------------------

void SScan::pauseCamera()
{
    m_pause = !m_pause;
}

//------------------------------------------------------------------------------

void SScan::presentDepthFrame()
{
    ::openni::VideoFrameRef depthFrame;
    int index;
    auto streamPtr = &m_depthStream;

    // grab depth & rgb
    while(::openni::OpenNI::waitForAnyStream(&streamPtr, 1, &index, 1000) == ::openni::STATUS_OK
          && m_rgbGrabber.isOpened())
    {
        if(m_pause)
        {
            continue;
        }

        m_depthStream.readFrame(&depthFrame);
        bool isGrabbed = m_rgbGrabber.grab();

        auto timestamp = std::chrono::duration_cast< std::chrono::milliseconds >
                             (std::chrono::system_clock::now().time_since_epoch()).count();

        if(depthFrame.isValid())
        {
            auto depthBuffer    = m_depthTL->createBuffer(timestamp);
            auto depthBufferPtr = reinterpret_cast<uint16_t*>(depthBuffer->addElement(0));
            auto depthPixels    = reinterpret_cast<const uint16_t*>(depthFrame.getData());
            memcpy(depthBufferPtr, depthPixels, static_cast<size_t>(depthFrame.getDataSize()));
            m_depthTL->pushObject(depthBuffer);

            m_depthTL->signal< ::arData::TimeLine::ObjectPushedSignalType>(::arData::TimeLine::s_OBJECT_PUSHED_SIG)
            ->asyncEmit(timestamp);

        }

        if (isGrabbed)
        {
            ::cv::Mat image;
            m_rgbGrabber.retrieve(image);

            // Get the buffer of the timeline to fill
            SPTR(::arData::FrameTL::BufferType) bufferOut = m_colorTL->createBuffer(timestamp);
            std::uint8_t* frameBuffOut = bufferOut->addElement(0);

            // Create an OpenCV mat that aliases the buffer created from the output timeline.
            ::cv::Mat imgOut(image.size(), CV_8UC4, (void*)frameBuffOut, ::cv::Mat::AUTO_STEP);

            ::cv::cvtColor(image, imgOut, ::cv::COLOR_BGR2RGBA);

            m_colorTL->pushObject(bufferOut);

            auto sig =
                m_colorTL->signal< ::arData::TimeLine::ObjectPushedSignalType >(::arData::TimeLine::s_OBJECT_PUSHED_SIG);
            sig->asyncEmit(timestamp);

        }

        if(depthFrame.isValid() && isGrabbed)
        {
            this->signal< ::arServices::IGrabber::FramePresentedSignalType>(
                ::arServices::IGrabber::s_FRAME_PRESENTED_SIG)
            ->asyncEmit();
        }

    }
}

//------------------------------------------------------------------------------

} // videoOrbbec
