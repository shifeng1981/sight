/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#ifndef __VISUVTKADAPTOR_SQSnapshot_HPP__
#define __VISUVTKADAPTOR_SQSnapshot_HPP__

#include "visuVTKAdaptor/config.hpp"

#include <fwCom/Slot.hpp>
#include <fwCom/Slots.hpp>

#include <fwRenderVTK/IQAdaptor.hpp>

namespace visuVTKAdaptor
{
/**
 * @brief This service will snapshot the current generic scene.
 *
 * It has two slots that can either snapshot into an ::fwData::Image or into a chosen image on the filesystem.
 *
 * @section Slots Slots
 * - \b snap(std::string filePath)    : This slot snaps onto the filesystem at a specified filepath.
 * - \b snapToImage()                 : This slot snaps into the configured image.
 *
 * @section QML QML configuration
 * @code{.qml}
   SSnapshot {
       image: ...
       config: ({
            renderer: "default"
       })
   }
   @endcode
 *
 * @subsection Outpu Output
 * - \b image [::fwData::Image] : Defines the fwData::Image to write into. Only used if snapToImage() is called.
 *
 * @subsection Configuration Configuration:
 * - \b config(mandatory) : contains the adaptor configuration
 *    - \b renderer (mandatory): defines the renderer to snapshot
 */

class VISUVTKADAPTOR_CLASS_API SQSnapshot : public ::fwRenderVTK::IQAdaptor
{

Q_OBJECT
public:

    VISUVTKADAPTOR_API SQSnapshot() noexcept;

    VISUVTKADAPTOR_API virtual ~SQSnapshot() noexcept;

protected:

    VISUVTKADAPTOR_API void configuring() override;
    VISUVTKADAPTOR_API void starting() override;
    VISUVTKADAPTOR_API void updating() override;
    VISUVTKADAPTOR_API void stopping() override;

private:

    /**
     * @name Slots
     * @{
     */
    /// Type of slot to snap shot
    static const ::fwCom::Slots::SlotKeyType s_SNAP_SLOT;

    /// Type of slot to snap shot to image
    static const ::fwCom::Slots::SlotKeyType s_SNAPTOIMAGE_SLOT;

public Q_SLOTS:
    /// Slot: snap shot the generic scene.
    void snap(QString const& filepath);

    /// Slot: snap shot the generic scene to fwData::Image
    void snapToImage();
    /**
     * @}
     */

private:
    /// UID of the image
    std::string m_imageUid;
};

} //namespace visuVTKAdaptor

#endif // __VISUVTKADAPTOR_SQSnapshot_HPP__
