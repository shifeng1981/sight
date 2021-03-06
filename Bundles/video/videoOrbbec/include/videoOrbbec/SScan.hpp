/************************************************************************
 *
 * Copyright (C) 2018-2019 IRCAD France
 * Copyright (C) 2018-2019 IHU Strasbourg
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

#pragma once

#include "videoOrbbec/config.hpp"
#include "videoOrbbec/SScanBase.hpp"

#include <arData/CameraSeries.hpp>
#include <arData/FrameTL.hpp>

#include <fwThread/Worker.hpp>

#include <opencv2/opencv.hpp>

#include <OpenNI.h>

namespace videoOrbbec
{

/**
 * @brief   Orbbec Astra Pro camera
 *
 * Service to grab video frames from the Orbbec Astra Pro camera.
 *
 * \b Tags: DEVICE
 *
 * @section Signals Signals
 * - \b positionModified(std::int64_t): Unimplemented.
 * - \b durationModified(std::int64_t): Unimplemented.
 *
 * @section Slots Slots
 * - \b startCamera(): Start capturing frames from the camera.
 * - \b stopCamera(): Stop capturing frames from the camera.
 * - \b pauseCamera(): Pause frame capture (the device is still opened though).
 * - \b toggleLoopMode(): Unimplemented.
 * - \b setPosition(int): Unimplemented.
 *
 * @section XML XML Configuration
 *
 * @code{.xml}
        <service uid="videoGrabber" type="::videoOrbbec::SScan">
            <inout key="depthTL" uid="..." />
            <inout key="frameTL" uid="..." />
            <videoMode>720p</videoMode>
        </service>
 * @endcode
 * @subsection In-Out In-Out
 * - \b depthTL [::arData::FrameTL]: timeline containing depth frames.
 * - \b frameTL [::arData::FrameTL]: timeline containing color frames.
 *
 * @subsection Configuration Configuration
 * \see SScanBase
 */
class VIDEOORBBEC_CLASS_API SScan : public SScanBase
{

public:
    fwCoreServiceClassDefinitionsMacro((SScan)(::arServices::IRGBDGrabber))

    /**
     * @brief SScan constructor
     */
    VIDEOORBBEC_API SScan() noexcept;

    /**
     * @brief SScan destructor
     */
    VIDEOORBBEC_API ~SScan() noexcept;

protected:
    /**
     * @brief Starts the service
     *
     * @throw ::fwTools::Failed
     */
    VIDEOORBBEC_API void starting() override;

    /**
     * @brief Starts capturing RGBD frames through a connected Astra camera.
     * Resets the stream if capture is already started.
     */
    void startCamera() override;

    /**
     * @brief Stops the capture.
     */
    void stopCamera() override;

    /**
     * @brief Pauses the capture.
     */
    void pauseCamera() override;

    /**
     * @brief Try detecting the cameras and setting them up, raise an exception if unable to.
     */
    void tryInitialize();

    /**
     * @brief Check whether the service is ready to read from the camera, ie all sensors are detected.
     */
    bool isReady() const noexcept;

private:
    // Timelines
    /// Timeline containing depth frames.
    ::arData::FrameTL::sptr m_depthTL;
    /// Timeline containing color frames.
    ::arData::FrameTL::sptr m_colorTL;

    /// RGB sensor settings.
    ::arData::Camera::csptr m_rgbCamera;

    // OpenNI types
    /// Error code of the last OpenNI call.
    ::openni::Status m_oniStatus;
    /// OpenNI stream representing the depth data stream.
    ::openni::VideoStream m_depthStream;

    /// Worker grabbing depth frames.
    ::fwThread::Worker::sptr m_workerDepth;

    /// Grabs color frames from the camera.
    ::cv::VideoCapture m_rgbGrabber;

    /// Whether we are in pause
    bool m_pause;

    /// True if the grabbing thread (presentDepthFrame()) should run. Set to false by stopCamera().
    bool m_isGrabbing {false};

    /// Protect m_isGrabbing, m_rgbGrabber, m_depthStream and m_pause, they run in the grabbing thread
    /// (presentDepthFrame()), and can by modified by this class.
    std::mutex m_grabbingMutex;

    // Slots
    /// Type of the presentDepthFrame slot.
    using PresentDepthFrameSlotType = ::fwCom::Slot<void()>;
    /// Slot for the presentDepthFrame method.
    PresentDepthFrameSlotType::sptr m_slotPresentDepthFrame;
    /**
     * @brief Slot: grabs depth frames from the camera and pushes them to their timeline.
     */
    void presentDepthFrame();
};

} // videoOrbbec
