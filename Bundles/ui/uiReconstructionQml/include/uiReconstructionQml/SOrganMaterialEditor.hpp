/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "uiReconstructionQml/config.hpp"

#include <fwQml/IQmlEditor.hpp>

#include <QColor>
#include <QObject>

namespace uiReconstructionQml
{

/**
 * @brief Display a widget to change the reconstruction material (color and transparency).
 *
 * @section QSignal Qt Signals
 * - \b materialChanged(QColor, int): emitted when the material changed (color, opacity)
 *
 * @section QSlots Qt Slots
 * - \b onOpacitySlider(int): called when the opacity changed, it will update the reconstruction Material
 * - \b onColor(QColor): called when the opacity changed, it will update the reconstruction Material

 *
 * @section QML Qml Configuration
 *
 * @code{.qml}
    SOrganMaterialEditor {
        id: organMaterialService

        onStarted: {
            organMaterialEditor.enabled = true
        }
        onStopped: {
            organMaterialEditor.enabled = false
        }

        onMaterialChanged: {
            colorDialog.setColor(color)
            transparencySlider.value = opacity
        }
    }
   @endcode
 *
 * @section Objects Required objects
 *
 * @subsection In-Out In-Out
 * - \b reconstruction [::fwData::Reconstruction]: reconstruction containing the material to update.
 */
class UIRECONSTRUCTIONQML_CLASS_API SOrganMaterialEditor : public ::fwQml::IQmlEditor
{
Q_OBJECT

public:

    fwCoreServiceClassDefinitionsMacro( (SOrganMaterialEditor)(::fwQml::IQmlEditor) )

    /// Constructor. Do nothing.
    UIRECONSTRUCTIONQML_API SOrganMaterialEditor() noexcept;

    /// Destructor. Do nothing.
    UIRECONSTRUCTIONQML_API virtual ~SOrganMaterialEditor() noexcept;

Q_SIGNALS:

    void materialChanged(QColor color, int opacity);

protected:

    typedef ::fwRuntime::ConfigurationElement::sptr Configuration;

    /// Call IQmlEditor::starting
    virtual void starting() override;

    /// Call IQmlEditor::stopping
    virtual void stopping() override;

    /// Emit a signal to update the Qml ui with the material information
    virtual void updating() override;

    /// Do nothing
    virtual void configuring() override;

    /**
     * @brief Returns proposals to connect service slots to associated object signals,
     * this method is used for obj/srv auto connection
     *
     * Connect Reconstruction::s_MODIFIED_SIG to this::s_UPDATE_SLOT
     */
    virtual KeyConnectionsMap getAutoConnections() const override;

protected Q_SLOTS:

    /// Called when the opacity changed, it will update the reconstruction Material
    void onOpacitySlider( int value);

    /// Called when the color changed, it will update the reconstruction Material
    void onColor(QColor color);

private:

    /// Notify the material changes
    void materialNotification();
};

} // uiReconstructionQml
