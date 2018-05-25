#include "fwServices/IQHasServices.hpp"
#include "fwServices/IQmlService.hpp"
#include "fwServices/QtQmlInstancier.hxx"


namespace fwServices
{

//IQHasServices::ServiceVectorGlobal IQHasServices::s_servicesList;

//------------------------------------------------------------------------------

IQHasServices::IQHasServices() noexcept
{
}

//------------------------------------------------------------------------------

IQHasServices::~IQHasServices() noexcept
{
    unregisterServices();
    SLM_ASSERT("Some sub-services were not unregistered, something is probably wrong. "
               "Please use unregisterService() or unregisterServices() before destroying the sub-services owner.",
               m_subServices.empty());
}

//------------------------------------------------------------------------------

::fwServices::IQmlService::csptr IQHasServices::getRegisteredService(const fwTools::fwID::IDType& _id) const
{
    std::shared_ptr< ::fwServices::IQmlService> srv;
    for(const auto& wService : m_subServices)
    {
        if(wService && (wService->getID() == _id))
        {
            srv = wService;
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
        if ((*itSrv)->getID() == _id)
        {
            (*itSrv)->stop();
            itSrv = m_subServices.erase(itSrv);
        }
        else
        {
            itSrv++;
        }
    }
}

//------------------------------------------------------------------------------

void IQHasServices::unregisterService(IQmlService::sptr _service)
{
    auto iter = std::find_if(m_subServices.begin(), m_subServices.end(),
                             [ = ](const std::shared_ptr<::fwServices::IQmlService>& adaptor)
        {
            return adaptor == _service;
        });

    SLM_ASSERT("service '" + _service->getID() + "' is not registered", iter != m_subServices.end());
    m_subServices.erase(iter);

    _service->stop();
}

//------------------------------------------------------------------------------

::fwServices::IQmlService::sptr IQHasServices::registerService(const std::string& _implType, const std::string& _id )
{
    auto srv = QtQmlInstancier::instanciate<::fwServices::IQmlService>(_implType);

    SLM_ASSERT("Service with type : <" + _implType + "> not found.", srv);
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
    for(auto itSrv = m_subServices.begin(); itSrv != m_subServices.end(); )
    {
        if (_classname.empty() || (!_classname.empty() && (*itSrv)->getClassname() == _classname))
        {
            (*itSrv)->stop();
            itSrv = m_subServices.erase(itSrv);
        }
        else
        {
            itSrv++;
        }
    }
}


}
