#pragma once

# include "fwServices/config.hpp"

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

class FWSERVICES_CLASS_API IQmlService : public QObject,
                                         public ::fwCom::HasSlots,
                                         public ::fwCom::HasSignals,
                                         public ::fwTools::fwID
{
	Q_OBJECT

public:
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
        CONFIGURED = 1
    };

    using KeyType = std::string;
    using KeyConnectionsType = ::fwCom::helper::SigSlotConnection::KeyConnectionsType;

    using ::fwTools::fwID::hasID;
    using ::fwTools::fwID::getID;
    using ::fwTools::fwID::setID;
    using ::fwTools::fwID::resetID;


public:
	// Ctor, do nothing
	FWSERVICES_API IQmlService();
	// Dtor, do nothing
	virtual FWSERVICES_API ~IQmlService();


    ServiceState 	getStatus() const;

    /**
     * @brief This class is a helper to define the connections of a service and its data.
     */
    class KeyConnectionsMap
    {
    public:
        //------------------------------------------------------------------------------

        void push (const KeyType& key,
                   const ::fwCom::Signals::SignalKeyType& sig,
                   const ::fwCom::Slots::SlotKeyType& slot)
        {
            m_keyConnectionsMap[key].push_back(std::make_pair(sig, slot));
        }

        typedef std::map< KeyType, KeyConnectionsType> KeyConnectionsMapType;


        //------------------------------------------------------------------------------

        KeyConnectionsMapType::const_iterator find(const KeyType& key) const
        {
            return m_keyConnectionsMap.find(key);
        }
        //------------------------------------------------------------------------------

        KeyConnectionsMapType::const_iterator end() const
        {
            return m_keyConnectionsMap.cend();
        }
        //------------------------------------------------------------------------------

        bool empty() const
        {
            return m_keyConnectionsMap.empty();
        }
        //------------------------------------------------------------------------------

        size_t size() const
        {
            return m_keyConnectionsMap.size();
        }
    private:
        std::map< KeyType, KeyConnectionsType> m_keyConnectionsMap;
    };

protected:
    /**
     *  @brief: This function should be override by children. It's called after a start()
     */
    virtual void FWSERVICES_API starting();
    /**
     *  @brief: This function should be override by children. It's called after a stop()
     */
    virtual void FWSERVICES_API stopping();
    /**
     *  @brief: This function should be override by children. It's called after a update()
     */
    virtual void FWSERVICES_API updating();
    /**
     *  @brief: This function should be override by children. It's called after a destroy()
     */
    virtual void FWSERVICES_API destroying();
    /**
     *  @brief: This function should be override by children. It's called after a configure()
     */
    virtual void FWSERVICES_API configuring();
    /**
     *  @brief: This function should be override by children. It's called after a reconfigure()
     */
    virtual void FWSERVICES_API reconfiguring();

    SPTR(::fwThread::Worker) m_associatedWorker;

	/**
	 *	@brief: Q_SLOTS functions list, that handle run/stop/destroy service
	 */
public Q_SLOTS:
	/**
	 *	@brief: start the service and needed work around data
	 *	This function must be call from subclasses
	 *	@post: emit started
	 */
    void FWSERVICES_API	start();
	/**
	 *	@brief: stop the service
	 *	@post: emit stopped
	 */
    void FWSERVICES_API	stop();
	/**
	 *	@brief: release service data (thread, etc.)
	 *	@post: emit destroyed()
	 */
    void FWSERVICES_API	destroy();
	/**
	 *	@brief: call to update service data
	 *	@post: emit updated() for service chain
	 */
    void FWSERVICES_API update();
    /**
     *  @brief: Used to configure service
     *  Call "configuring" or "reconfiguring" function depend of current configuration state
     */
    void FWSERVICES_API configure();


    /**
     * Configuration parsing helpers
     */
protected:
    template<typename T>
    T const    get(QVariantMap const& target, std::string const& key, T const& defaultValue);
    template<typename T>
    T const    get(QVariantMap const& target, std::string const& key);



public:
    /**
     *  @brief: Exposition of generic qml's properties
     */
    void    FWSERVICES_API setProperty(const std::string& name, const QVariant& value);

    /**
     *  @brief: T object stored in a QtObjectHolder by propertyName
     *  This object usually is a derived class of "::fwData::Object"
     */
    template<typename T>
    typename T::sptr     getInOut(const std::string& propertyName) const;

Q_SIGNALS:
	// Extern signals
	void	started();
	void	updated();
	void	stopped();
	void	destroyed();

private:
    ServiceState	m_serviceState = STOPPED;
    ConfigurationState  m_configurationState = NOT_CONFIGURED;
};

} // fwServices

#include "fwServices/IQmlService.hxx"
