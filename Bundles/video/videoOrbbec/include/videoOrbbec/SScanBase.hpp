/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

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
