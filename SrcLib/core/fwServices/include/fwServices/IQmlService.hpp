#pragma once

# include "fwServices/config.hpp"

# include <QObject>

#ifndef emit
# define emit
#endif // !emit

namespace fwServices
{

class FWSERVICES_CLASS_API IQmlService : public QObject
{
	Q_OBJECT

public:
	enum ServiceState
	{
		STARTED = 0,
		STOPPED = 1,
		DESTROYED = 2
	};

    enum ConfigurationState
    {
        NOT_CONFIGURED = 0,
        CONFIGURED = 1
    };

public:
	// Ctor, do nothing
	FWSERVICES_API IQmlService();
	// Dtor, do nothing
	virtual FWSERVICES_API ~IQmlService();


	ServiceState 	getState() const;

    /**
     *  @brief: This function must be override by children. It's called after a start()
     */
    virtual void FWSERVICES_API starting() = 0;
    /**
     *  @brief: This function must be override by children. It's called after a stop()
     */
    virtual void FWSERVICES_API stopping() = 0;
    /**
     *  @brief: This function must be override by children. It's called after a update()
     */
    virtual void FWSERVICES_API updating() = 0;
    /**
     *  @brief: This function must be override by children. It's called after a destroy()
     */
    virtual void FWSERVICES_API destroying() = 0;
    /**
     *  @brief: This function must be override by children. It's called after a configure()
     */
    virtual void FWSERVICES_API configuring() = 0;
    /**
     *  @brief: This function must be override by children. It's called after a reconfigure()
     */
    virtual void FWSERVICES_API reconfiguring() = 0;

	
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
