#include "fwServices/IQmlService.hpp"

namespace fwServices
{

IQmlService::IQmlService() : m_serviceState(STOPPED)
{
}

IQmlService::~IQmlService()
{
}

void	IQmlService::start()
{
	// Connect in/out method
	connect(this, SIGNAL(inSet()), this, SLOT(inChange()));
	connect(this, SIGNAL(outSet()), this, SLOT(outChange()));
	m_serviceState = STARTED;
	emit started();
}

void	IQmlService::update()
{
	emit updated();
}

void	IQmlService::stop()
{
	SLM_ASSERT("Service isn't running", m_serviceState == STARTED);
	m_serviceState = STOPPED;
	emit stopped();
}

void	IQmlService::destroy()
{
	m_serviceState = DESTROYED;
	emit destroyed();
}

IQmlService::ServiceState 	IQmlService::getState() const
{
	return m_serviceState;
}

} // fwServices
