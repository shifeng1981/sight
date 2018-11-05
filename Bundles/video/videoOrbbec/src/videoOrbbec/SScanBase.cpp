/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "videoOrbbec/SScanBase.hpp"

#include <arData/CameraSeries.hpp>

#include <fwGui/dialog/MessageDialog.hpp>

#include <fwTools/Os.hpp>

#include <fwVideoQt/helper/formats.hpp>
#include <fwVideoQt/Registry.hpp>

#include <QCamera>
#include <QCameraInfo>

#include <cmath>
#include <regex>
#include <string>

namespace videoOrbbec
{

const std::string SScanBase::s_QT_CAMERA_STRING = "Astra Pro HD Camera";

//------------------------------------------------------------------------------

SScanBase::SScanBase() noexcept :
    m_oniDevice(nullptr),
    m_720p(false)
{
}

//------------------------------------------------------------------------------

SScanBase::~SScanBase() noexcept
{
}

//------------------------------------------------------------------------------

void SScanBase::configuring()
{
    ::fwRuntime::ConfigurationElement::sptr videoModeConfig =
        m_configuration->findConfigurationElement("videoMode");
    if(videoModeConfig)
    {
        if(videoModeConfig->getValue() == "720p")
        {
            m_720p = true;
        }
        else if(videoModeConfig->getValue() == "VGA")
        {
            m_720p = false;
        }
        else
        {
            SLM_FATAL("Unknown value for the 'videoMode' attribute: " + videoModeConfig->getValue()
                      + ". Valid values are '720p' and 'VGA'.");
        }
    }
}

//------------------------------------------------------------------------------

void SScanBase::stopping()
{
    this->stopCamera();
}

//------------------------------------------------------------------------------

void SScanBase::updating()
{
}

//------------------------------------------------------------------------------

void SScanBase::pauseCamera()
{
}

//------------------------------------------------------------------------------

void SScanBase::toggleLoopMode()
{
}

//------------------------------------------------------------------------------

void SScanBase::setPosition(int64_t position)
{
}

//------------------------------------------------------------------------------

void SScanBase::detectCameraOpenni()
{

    auto cameraInput = this->getInput< ::fwData::Object >(s_CAMERA_INPUT);
    auto camera      = ::arData::Camera::dynamicConstCast(cameraInput);

    std::string camId;
    int index = -1;

    if(camera)
    {
        camId = camera->getCameraID();
    }
    else
    {
        auto cameraSeries = ::arData::CameraSeries::dynamicConstCast(cameraInput);
        if(cameraSeries)
        {
            size_t numCamerasInSeries = cameraSeries->getNumberOfCameras();
            SLM_ASSERT("Camera Series is empty", numCamerasInSeries);

            // Assume same source on all cameras
            camId = cameraSeries->getCamera(0)->getCameraID();
            index = cameraSeries->getCamera(0)->getIndex();
        }
    }

    // Convert camID from Qt (/dev/video#) to an usb device number.
    //Note: For compatibilities issues, call this from an other OS than Linux leads to
    // usbDevises[0] = std::pair[-1; "ERROR"]
    auto usbDevices = ::fwTools::os::getDeviceFromVirtualDevice(camId);

    ::openni::Array< ::openni::DeviceInfo> devices;
    ::openni::OpenNI::enumerateDevices(&devices);
    OpenniDevicePtr device = std::unique_ptr< ::openni::Device >(new ::openni::Device());
    std::string astraDeviceUri;

    bool foundAstra = false;

    for(int i = 0; i != devices.getSize(); ++i)
    {
        auto const& dev = devices[i];
        if(!(std::strcmp("Orbbec", dev.getVendor()) || std::strcmp("Astra", dev.getName())
             || dev.getUsbVendorId() != 11205))
        {

            astraDeviceUri = dev.getUri();

            // if we found the correspondence between /dev/video# uri and the device id (Can only be true on linux)
            if(usbDevices[0].first != -1)
            {
                for(auto it : usbDevices)
                {
                    std::string astraNum = astraDeviceUri.substr(astraDeviceUri.rfind("/") + 1 );

                    if(it.first == std::stoi((astraNum)))
                    {
                        // ok we found the corresponding astra, stop the loop

                        SLM_DEBUG("OpenNI URI: " + astraDeviceUri);
                        SLM_DEBUG("Qt Uri: " + camId);
                        OSLM_DEBUG("Matching found: " + it.second + " devnum: "<<it.first);

                        foundAstra = true;
                        // break the inner loop.
                        break;
                    }
                }
            }

            if(foundAstra)
            {
                // ok found astra break the main loop
                break;
            }
            else
            {
                //Default case: assume that astra are organized in the same order than Qt
                if(index == i && index >= 0)
                {
                    foundAstra = true;
                    break;
                }
                // Last case last astra found is used...
            }
        }
    }

    FW_RAISE_IF("Astra Pro OpenNI device not detected.", astraDeviceUri.size() == 0);
    FW_RAISE_IF("Cannot open OpenNI device: " << ::openni::OpenNI::getExtendedError(),
                device->open(astraDeviceUri.c_str()) != ::openni::STATUS_OK);

    m_oniDevice.swap(device);
    m_oniDevice->setImageRegistrationMode(
        ::openni::ImageRegistrationMode::IMAGE_REGISTRATION_OFF);
}

//------------------------------------------------------------------------------

::arData::Camera::sptr SScanBase::detectCameraQt()
{

    //FIXME: useless if using ::arData::Camera.
    // Workaround https://bugreports.qt.io/browse/QTBUG-59320
    QCamera* cam = new QCamera;
    delete cam;

    const auto qtCameras = QCameraInfo::availableCameras();
    bool foundAstra      = false;
    auto cameraData      = ::arData::Camera::New();
    for(auto const& it: qtCameras)
    {
        OSLM_DEBUG("Setting video mode to " << (m_720p ? "720p" : "VGA"));
        // Some OS's (hem OS X) append semi-random numbers to the camera name depending on which USB port
        // you plug it in. So, we extract a substring of desired length to get rid of that
        auto camName = it.description().toStdString();
        if(camName.length() > s_QT_CAMERA_STRING.length())
        {
            camName = camName.substr(0, s_QT_CAMERA_STRING.length());
        }
        else if(camName.length() < s_QT_CAMERA_STRING.length())
        {
            continue;
        }
        if(camName == s_QT_CAMERA_STRING)
        {
            // Found a match, pick a video mode and fill the arData::Camera
            foundAstra = true;
#ifdef WIN32
            QCamera qtCamera(it);
            qtCamera.load();
            const auto settingsList = qtCamera.supportedViewfinderSettings();
            const auto settings     =
                std::find_if(settingsList.begin(), settingsList.end(),
                             [&] (auto& s)
                {
                    auto res = s.resolution();
                    if(m_720p)
                    {
                        return res.width() == 1280
                        && res.height() == 720
                        && std::lround(s.minimumFrameRate()) == 10;
                    }
                    else
                    {
                        return res.width() == 640
                        && res.height() == 480
                        && std::lround(s.minimumFrameRate()) == 30;
                    }
                });
            FW_RAISE_IF("No valid videomodes detected", settings == settingsList.end());
            const auto resolution = settings->resolution();
            const auto formatIter = ::fwVideoQt::helper::pixelFormatTranslator.left.find(settings->pixelFormat());
            FW_RAISE_IF("No compatible pixel format detected",
                        formatIter == ::fwVideoQt::helper::pixelFormatTranslator.left.end());

            // startCamera needs to know the frame size, so we set it here
            cameraData->setWidth(resolution.width());
            cameraData->setHeight(resolution.height());
            cameraData->setMaximumFrameRate(settings->maximumFrameRate());
#else
            // This is a temporary workaround against Qt multimedia not being able to load
            // the camera on linux until we can find a proper solution...
            cameraData->setWidth(m_720p ? 1280 : 640);
            cameraData->setHeight(m_720p ? 720 : 480);
            cameraData->setMaximumFrameRate(m_720p ? 10 : 30);
#endif
            cameraData->setCameraSource(::arData::Camera::DEVICE);
            cameraData->setDescription(s_QT_CAMERA_STRING);
            cameraData->setCameraID(it.deviceName().toStdString());
            break;
        }
    }
    return cameraData;
}

//------------------------------------------------------------------------------

void SScanBase::errorPopup(std::string const& msg)
{
    ::fwGui::dialog::MessageDialog::showMessageDialog(
        "videoOrbbec error",
        msg,
        ::fwGui::dialog::IMessageDialog::CRITICAL);
}

//------------------------------------------------------------------------------

void SScanBase::qtMessageHandler(QtMsgType type, QMessageLogContext const& ctx, QString const& msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type)
    {
        case QtDebugMsg:
            SLM_DEBUG(localMsg.constData());
            break;
        case QtInfoMsg:
            SLM_INFO(localMsg.constData());
            break;
        case QtWarningMsg:
            SLM_WARN(localMsg.constData());
            break;
        case QtCriticalMsg:
            SLM_ERROR(localMsg.constData());
            break;
        case QtFatalMsg:
            SLM_FATAL(localMsg.constData());
            break;
    }
}

} // namespace videoOrbbec
