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

	/**
	 * QML expose in/out
	 */
	Q_PROPERTY(bool in MEMBER m_dataIn NOTIFY inSet);
	Q_PROPERTY(bool out MEMBER m_dataOut NOTIFY outSet);

public:
	enum ServiceState
	{
		STARTED = 0,
		STOPPED = 1,
		DESTROYED = 2
	};

public:
	// Ctor, do nothing
	FWSERVICES_API IQmlService();
	// Dtor, do nothing
	virtual FWSERVICES_API ~IQmlService();


	ServiceState 	getState() const;
	
	/**
	 *	@brief: Q_SLOTS functions list, that handle run/stop/destroy service
	 */
public Q_SLOTS:
	/**
	 *	@brief: start the service and needed work around data
	 *	This function must be call from subclasses
	 *	@post: emit started
	 */
	virtual void FWSERVICES_API	start();
	/**
	 *	@brief: stop the service
	 *	@post: emit stopped
	 */
	virtual void FWSERVICES_API	stop();
	/**
	 *	@brief: release service data (thread, etc.)
	 *	@post: emit destroyed()
	 */
	virtual void FWSERVICES_API	destroy();
	/**
	 *	@brief: call to update service data
	 *	@post: emit updated() for service chain
	 */
	virtual void FWSERVICES_API update();

private Q_SLOTS:
	/**
	 *	@brief: Slot will link to signal Data::changed() and inSet()
	 */
	virtual void FWSERVICES_API inChange() = 0;
	/**
	 *	@brief: Slot will link to outSet
	 */
	virtual void FWSERVICES_API outChange() = 0;

Q_SIGNALS:
	// Extern signals
	void	started();
	void	updated();
	void	stopped();
	void	destroyed();

	// Intra signals
	void	inSet();
	void	outSet();

private:
	bool	m_dataIn;
	bool	m_dataOut;
	ServiceState	m_serviceState;
};

} // fwServices
