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
	m_serviceState = STARTED;
    this->starting();
	emit started();
}

void	IQmlService::update()
{
    this->updating();
	emit updated();
}

void    IQmlService::configure()
{
    if (m_configurationState == NOT_CONFIGURED)
    {
        try {
            this->configuring();
        }
        catch (std::exception& e) {
            SLM_ERROR(std::string("Error while configuring service: '") + e.what());
        }
    }
    else {
        this->reconfiguring();
    }
}

void	IQmlService::stop()
{
	SLM_ASSERT("Service isn't running", m_serviceState == STARTED);
	m_serviceState = STOPPED;
    this->stopping();
	emit stopped();
}

void	IQmlService::destroy()
{
	m_serviceState = DESTROYED;
    this->destroying();
	emit destroyed();
}

IQmlService::ServiceState 	IQmlService::getState() const
{
	return m_serviceState;
}

} // fwServices
