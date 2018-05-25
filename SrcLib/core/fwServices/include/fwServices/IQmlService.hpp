/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

# include "fwServices/config.hpp"
# include "fwServices/macros.hpp"

#include <fwThread/Worker.hpp>

# include <QObject>
# include <QVariantMap>

#include <fwCom/helper/SigSlotConnection.hpp>
#include <fwCom/HasSignals.hpp>
#include <fwCom/HasSlots.hpp>
#include <fwCom/Slot.hpp>
#include <fwCom/Slots.hpp>

#include <fwTools/fwID.hpp>

namespace fwServices
{

class FWSERVICES_CLASS_QT_API IQmlService : public QObject,
                                            public ::fwCom::HasSlots,
                                            public ::fwCom::HasSignals,
                                            public ::fwTools::fwID
{
Q_OBJECT

/**
 *  Exposure of C++ property to QML
 *  @{
 */
Q_PROPERTY(QVariantMap config MEMBER m_configMap)
Q_PROPERTY(bool autoStart MEMBER m_autoStart)
/**
 * }@
 */

public:

    fwQmlTypeMacro(IQmlService);

    enum ServiceState
    {
        STARTED,    /**< state after start */
        STARTING,   /**< state during start */
        STOPPED,    /**< state after stop */
        STOPPING,    /**< state during stop */
        DESTROYED
    };

    enum ConfigurationState
    {
        NOT_CONFIGURED = 0,
        CONFIGURED     = 1
    };

    using KeyType            = std::string;
    using KeyConnectionsType = ::fwCom::helper::SigSlotConnection::KeyConnectionsType;

    using ::fwTools::fwID::hasID;
    using ::fwTools::fwID::getID;
    using ::fwTools::fwID::setID;
    using ::fwTools::fwID::resetID;

public:
    // Ctor, do nothing
    FWSERVICES_QT_API IQmlService();
    // Dtor, do nothing
    virtual FWSERVICES_QT_API ~IQmlService();

    ServiceState getStatus() const;

protected:
    /**
     *  @brief: This function should be override by children. It's called after a start()
     */
    virtual FWSERVICES_QT_API void starting();
    /**
     *  @brief: This function should be override by children. It's called after a stop()
     */
    virtual FWSERVICES_QT_API void stopping();
    /**
     *  @brief: This function should be override by children. It's called after a update()
     */
    virtual FWSERVICES_QT_API void updating();
    /**
     *  @brief: This function should be override by children. It's called after a destroy()
     */
    virtual FWSERVICES_QT_API void destroying();
    /**
     *  @brief: This function should be override by children. It's called after a configure()
     */
    virtual FWSERVICES_QT_API void configuring();
    /**
     *  @brief: This function should be override by children. It's called after a reconfigure()
     */
    virtual FWSERVICES_QT_API void reconfiguring();

    SPTR(::fwThread::Worker) m_associatedWorker;

    /**
     *  @brief: Q_SLOTS functions list, that handle run/stop/destroy service
     */
public Q_SLOTS:
    /**
     *  @brief: start the service and needed work around data
     *  This function must be call from subclasses
     *  @post: emit started
     */
    FWSERVICES_QT_API void start();
    /**
     *  @brief: stop the service
     *  @post: emit stopped
     */
    FWSERVICES_QT_API void stop();
    /**
     *  @brief: release service data (thread, etc.)
     *  @post: emit destroyed()
     */
    FWSERVICES_QT_API void destroy();
    /**
     *  @brief: call to update service data
     *  @post: emit updated() for service chain
     */
    FWSERVICES_QT_API void update();
    /**
     *  @brief: Used to configure service
     *  Call "configuring" or "reconfiguring" function depend of current configuration state
     */
    FWSERVICES_QT_API void configure();

    /**
     *  @brief: Check if the service is currently started.
     *  m_serviceStatus == STARTED
     */
    FWSERVICES_QT_API bool isStarted() const;

    /**
     *  @brief: Check if the service is configured with autoStart mode
     */
    FWSERVICES_QT_API bool isAutoStart() const;

    /**
     * Configuration parsing helpers
     */
protected:
    /**
     *  @brief: get helper to retrieve parameters from a QVariantMap (e.g m_configMap)
     *
     *  Config map in qml looks like :
     *  SService {
     *      config: ({
     *          "property": "value"
     *          "anotherProperty": 23
     *      })
     *  }
     */

    /**
     *  @brief: In case of `key` is not found, defaultValue is returned.
     */
    template<typename T>
    T const get(QVariantMap const& target, std::string const& key, T const& defaultValue);
    /**
     *  @brief: In case of `key` is not found, a T instance is returned.
     */
    template<typename T>
    T const get(QVariantMap const& target, std::string const& key);

    /**
     *  Qml properties
     *  @{
     */
    QVariantMap m_configMap;
    bool m_autoStart = false;
    /**
     * }@
     */

public:
    /**
     *  @brief: Exposition of generic qml's properties
     *  This method allows us to set property at runtime without having a reference to the variable.
     *  If the property `name` doesn't exists, do nothing.
     */
    void FWSERVICES_QT_API setProperty(const std::string& name, const QVariant& value);

    /**
     *  @brief: T object stored in a QtObjectHolder by propertyName
     *  This object usually is a derived class of "::fwData::Object"
     */
    template<typename T>
    typename T::sptr getInOut(const std::string& propertyName) const;

Q_SIGNALS:
    // Extern signals
    void started();
    void updated();
    void stopped();
    void destroyed();

private:
    ServiceState m_serviceState             = STOPPED;
    ConfigurationState m_configurationState = NOT_CONFIGURED;

};

} // fwServices

#include "fwServices/IQmlService.hxx"
