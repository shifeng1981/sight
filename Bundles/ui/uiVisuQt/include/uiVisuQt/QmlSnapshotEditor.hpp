/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "uiVisuQt/config.hpp"

#include <fwCom/Signal.hpp>
#include <fwCom/Signals.hpp>

#include <fwGui/editor/IEditor.hpp>

#include <fwServices/IQmlService.hpp>

#include <fwTools/Failed.hpp>

#include <QObject>
#include <QPointer>

class QPushButton;

namespace uiVisuQt
{

/**
 * @brief   QmlSnapshotEditor service is represented by a button. It allows to snap shot a generic scene.
 *
 * Send a 'snapped' signal containing the filename used to save the snapshot.
 * @note You need to connect the 'snapped' signal to one visuVTKAdaptor::Snapshot to save the file.
 */
class UIVISUQT_CLASS_API QmlSnapshotEditor : public ::fwServices::IQmlService
{

Q_OBJECT

public:

    /// Constructor. Do nothing.
    UIVISUQT_API QmlSnapshotEditor() noexcept;

    /// Destructor. Do nothing.
    UIVISUQT_API virtual ~QmlSnapshotEditor() noexcept;

protected:

    typedef ::fwRuntime::ConfigurationElement::sptr Configuration;

    /**
     * @brief Install the layout.
     */
    virtual void starting() override;

    /**
     * @brief Destroy the layout.
     */
    virtual void stopping() override;

    /// Do nothing
    virtual void updating() override;

    /**
     * @brief Configure the editor.
     *
     * Example of configuration
     * @code{.xml}
       <service uid="QmlSnapshotEditor" type="::uiVisuQt::QmlSnapshotEditor" autoConnect="no" />
       @endcode
     */
    virtual void configuring() override;

protected Q_SLOTS:
    /**
     * @brief Show a file dialog and notify the scene must be printed.
     */
    void onSnapButton();

private:
    std::string requestFileName();

Q_SIGNALS:
    void    snapped(QString const& filepath);

};

} // uiVisuQt
