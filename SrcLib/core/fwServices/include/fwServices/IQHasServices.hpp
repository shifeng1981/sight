#ifndef IQHASSERVICE_H
#define IQHASSERVICE_H

#include "fwServices/config.hpp"
#include "fwServices/QtQmlInstancier.hxx"

#include <fwCore/macros.hpp>

#include <vector>

namespace fwServices
{
class IQmlService;

/**
 * @brief   Interface for objects that need to manage services.
 */
class FWSERVICES_CLASS_API IQHasServices
{
public:

    typedef std::vector < SPTR(::fwServices::IQmlService) > ServiceVector;
    using ServiceVectorGlobal = std::vector< SPTR(::fwServices::IQmlService) >;

    /**
     * @brief Get all subservices linked to this service
     * @return The vector of linked services
     */
    const ServiceVector& getRegisteredServices() const;

    /**
     * @brief Return a specific registered service
     * @param[in] id Identifier of the service
     */
    FWSERVICES_API CSPTR( ::fwServices::IQmlService) getRegisteredService(const ::fwTools::fwID::IDType& _id) const;

protected:

    /// Constructor
    FWSERVICES_API IQHasServices() noexcept;

    /// Destructor
    FWSERVICES_API virtual ~IQHasServices() noexcept;

    /**
     * @brief Register a new service linked to this service
     * @param[in] _implType Type of the service
     * @param[in] id Optional identifier of the service
     */
    FWSERVICES_API SPTR(::fwServices::IQmlService) registerService(const std::string& _implType,
                                                                 const std::string& _id = "" );

    /**
     * @brief Register a new service linked to this service
     * @param[in] _implType Type of the service
     * @param[in] id Optional identifier of the service
     */
    template <class T>
    SPTR(T) registerService(const std::string& _implType, const std::string& _id = "" );

    /**
     * @brief Unregister a specific service
     * @param id Identifier of the service
     */
    FWSERVICES_API void unregisterService(const ::fwTools::fwID::IDType& _id);

    /**
     * @brief Unregister a specific service
     * @param service Pointer to the service
     */
    FWSERVICES_API void unregisterService(const std::shared_ptr<::fwServices::IQmlService>& _service);

    /**
     * @brief Unregister all services linked to this service, optionally matches only a given type of services
     * @param _implType Optional type of the services to unregister
     */
    FWSERVICES_API void unregisterServices(const std::string& _implType = "");

private:

    /// Sub services linked to this service
    ServiceVector m_subServices;

    //static ServiceVectorGlobal s_servicesList;
};

//------------------------------------------------------------------------------

inline const IQHasServices::ServiceVector& IQHasServices::getRegisteredServices() const
{
    return m_subServices;
}

//------------------------------------------------------------------------------

template <class T>
SPTR(T) IQHasServices::registerService(const std::string& _implType, const std::string& _id)
{
    auto srv = QtQmlInstancier::instanciate<T>(_implType);

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

} //namespace fwServices

#endif // IQHASSERVICE_H
