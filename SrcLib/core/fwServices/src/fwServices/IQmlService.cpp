/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwServices/IQmlService.hpp"

#include "fwServices/registry/ActiveWorkers.hpp"

namespace fwServices
{

IQmlService::IQmlService() :
    m_serviceState(STOPPED)
{
    m_associatedWorker = ::fwServices::registry::ActiveWorkers::getDefaultWorker();
}

IQmlService::~IQmlService()
{
    if (m_serviceState == STARTED)
    {
        this->stop();
    }
}

//------------------------------------------------------------------------------

void IQmlService::start()
{
    m_serviceState = STARTED;
    ::fwCom::HasSlots::m_slots.setWorker( m_associatedWorker );

    this->starting();
    started();
}

//------------------------------------------------------------------------------

void IQmlService::update()
{
    this->updating();
    updated();
}

//------------------------------------------------------------------------------

void IQmlService::configure()
{
    if (m_configurationState == NOT_CONFIGURED)
    {
        try
        {
            this->configuring();
        }
        catch (std::exception& e)
        {
            SLM_ERROR(std::string("Error while configuring service: '") + e.what());
        }
    }
    else
    {
        this->reconfiguring();
    }
}

//------------------------------------------------------------------------------

void IQmlService::stop()
{
    SLM_ASSERT("Service isn't running", m_serviceState == STARTED);
    m_serviceState = STOPPED;
    this->stopping();
    stopped();
}

//------------------------------------------------------------------------------

void IQmlService::destroy()
{
    m_serviceState = DESTROYED;
    this->destroying();
    destroyed();
}

//------------------------------------------------------------------------------

IQmlService::ServiceState IQmlService::getStatus() const
{
    return m_serviceState;
}

//------------------------------------------------------------------------------

void IQmlService::starting()
{
}

//------------------------------------------------------------------------------

void IQmlService::stopping()
{
}

//------------------------------------------------------------------------------

void IQmlService::updating()
{
}

//------------------------------------------------------------------------------

void IQmlService::configuring()
{
}

//------------------------------------------------------------------------------

void IQmlService::reconfiguring()
{
}

//------------------------------------------------------------------------------

void IQmlService::destroying()
{
}

//------------------------------------------------------------------------------

bool IQmlService::isStarted() const
{
    return m_serviceState == STARTED;
}

//------------------------------------------------------------------------------

bool IQmlService::isAutoStart() const
{
    return m_autoStart;
}

//------------------------------------------------------------------------------

void IQmlService::setProperty(const std::string& name, const QVariant& value)
{
    QObject::setProperty(name.c_str(), value);
}

} // fwServices
