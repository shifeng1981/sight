/************************************************************************
 *
 * Copyright (C) 2018 IRCAD France
 * Copyright (C) 2018 IHU Strasbourg
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

#include <arData/Camera.hpp>

#include <arServices/IRGBDGrabber.hpp>

#include <fwCom/Slot.hpp>
#include <fwCom/Slot.hxx>
#include <fwCom/Slots.hxx>

#include <OpenNI.h>

#include <memory>

namespace videoOrbbec
{

class ColorFrameWorker;

/**
 * @brief Base class for Orbbec Astra Pro grabbers.
 *
 * @section Signals Signals
 * - \b positionModified(std::int64_t): Unimplemented.
 * - \b durationModified(std::int64_t): Unimplemented.
 *
 * @section Slots Slots
 * - \b startCamera(): Start capturing frames from the camera.
 * - \b stopCamera(): Stop capturing frames from the camera.
 * - \b pauseCamera(): Unimplemented.
 * - \b toggleLoopMode(): Unimplemented.
 * - \b setPosition(int): Unimplemented.
 *
 * @section XML XML Configuration
 *
 * @code{.xml}
        <service uid="videoGrabber" type="...">
        </service>
 * @endcode
 *
 */
class VIDEOORBBEC_CLASS_API SScanBase : public ::arServices::IRGBDGrabber
{
public:
    fwCoreNonInstanciableClassDefinitionsMacro((SScanBase)(::arServices::IRGBDGrabber))

    /**
     * @brief SScanBase constructor.
     */
    VIDEOORBBEC_API SScanBase() noexcept;

    /**
     * @brief SScanBase destructor.
     */
    VIDEOORBBEC_API ~SScanBase() noexcept;

    /// OpenNI2 device pointer type with unique ownership.
    using OpenniDevicePtr = std::unique_ptr< ::openni::Device>;

protected:
    /**
     * @brief Configures the service.
     *
     * @throw fwTools::Failed
     */
    VIDEOORBBEC_API void configuring() override;

    /**
     * @brief Stops the service
     *
     * @throw ::fwTools::Failed
     */
    VIDEOORBBEC_API void stopping() override;

    /**
     * @brief Does nothing
     *
     * @throw ::fwTools::Failed
     */
    VIDEOORBBEC_API void updating() override;

    /**
     * @brief Not implemented
     */
    VIDEOORBBEC_API void pauseCamera() override;

    /**
     * @brief Not implemented
     */
    VIDEOORBBEC_API void toggleLoopMode() override;

    /**
     * @brief Not implemented
     */
    VIDEOORBBEC_API void setPosition(int64_t position) override;

    /**
     * @brief Enumerate connected OpenNI devices and search for the Orbbec Astra Pro.
     */
    VIDEOORBBEC_API void detectCameraOpenni();

    /**
     * @brief Display a Qt error popup displaying the error string passed as argument.
     *
     * @param msg
     */
    VIDEOORBBEC_API static void errorPopup(std::string const& msg);

    /// OpenNI device representing the Orbbec Astra Pro.
    OpenniDevicePtr m_oniDevice;
};

} // videoOrbbec
