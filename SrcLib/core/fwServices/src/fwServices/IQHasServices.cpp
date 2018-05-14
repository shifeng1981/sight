#include "fwServices/IQHasServices.hpp"
#include "fwServices/IQmlService.hpp"
#include "fwServices/QtQmlInstancier.hxx"


namespace fwServices
{

IQHasServices::ServiceVectorGlobal IQHasServices::s_servicesList;

//------------------------------------------------------------------------------

IQHasServices::IQHasServices() noexcept
{
}

//------------------------------------------------------------------------------

IQHasServices::~IQHasServices() noexcept
{
    SLM_ASSERT("Some sub-services were not unregistered, something is probably wrong. "
               "Please use unregisterService() or unregisterServices() before destroying the sub-services owner.",
               m_subServices.empty());
}

//------------------------------------------------------------------------------

std::shared_ptr<const ::fwServices::IQmlService> IQHasServices::getRegisteredService(const fwTools::fwID::IDType& _id) const
{
    std::shared_ptr<::fwServices::IQmlService> srv;
    for(const auto& wService : m_subServices)
    {
        const std::shared_ptr<::fwServices::IQmlService>& service = wService.lock();
        if(service && (service->getID() == _id))
        {
            srv = service;
            break;
        }
    }
    return srv;
}

//------------------------------------------------------------------------------

void IQHasServices::unregisterService(const fwTools::fwID::IDType& _id)
{
    for(auto itSrv = m_subServices.begin(); itSrv != m_subServices.end(); )
    {
        const std::shared_ptr<::fwServices::IQmlService>& service = itSrv->lock();
        if(service && (service->getID() == _id))
        {
            service->stop();
            itSrv = m_subServices.erase(itSrv);
        }
        else
        {
            itSrv++;
        }
    }
}

//------------------------------------------------------------------------------

void IQHasServices::unregisterService(const std::shared_ptr<IQmlService>& _service)
{
    auto iter = std::find_if(m_subServices.begin(), m_subServices.end(),
                             [ = ](const std::weak_ptr<::fwServices::IQmlService>& adaptor)
        {
            return adaptor.lock() == _service;
        });

    SLM_ASSERT("service '" + _service->getID() + "' is not registered", iter != m_subServices.end());
    m_subServices.erase(iter);

    _service->stop();
}

//------------------------------------------------------------------------------

std::shared_ptr<::fwServices::IQmlService> IQHasServices::registerService(const std::string& _implType, const std::string& _id )
{
    auto srv = QtQmlInstancier::instanciate<::fwServices::IQmlService>(_implType);

    if(!_id.empty())
    {
        SLM_ASSERT( "Try to set ID: " + _id + " but already has an ID: " + srv->getID(), !srv->hasID() );
        srv->setID( _id );
    }
    m_subServices.push_back(srv);

    return srv;
}

//------------------------------------------------------------------------------

void IQHasServices::unregisterServices(const std::string& _classname)
{
    std::cout << s_servicesList.size() << std::endl;
    std::cout << m_subServices.size() << std::endl;
    for(const auto& wService : m_subServices)
    {
        const std::shared_ptr<::fwServices::IQmlService>& service = wService.lock();

        if (service && (_classname.empty() || (!_classname.empty() && service->getClassname() == _classname)))
        {
            service->stop();
        }
    }
    m_subServices.clear();
}


}
