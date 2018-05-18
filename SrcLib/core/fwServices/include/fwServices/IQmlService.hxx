#ifndef IQMLSERVICE_HXX
#define IQMLSERVICE_HXX

#include "fwServices/QtObjectHolder.hpp"

namespace fwServices
{

template<typename T>
T const    IQmlService::get(QVariantMap const& target, std::string const& key, T const& defaultValue)
{
    if (target.find(key.c_str()) != target.end())
    {
        return target[key.c_str()].value<T>();
    }
    return defaultValue;
}

template<typename T>
T const    IQmlService::get(QVariantMap const& target, std::string const& key)
{
    if (target.find(key.c_str()) != target.end())
    {
        return target[key.c_str()].value<T>();
    }
    return T();
}

template<typename T>
typename T::sptr IQmlService::getInOut(const std::string& propertyName) const
{
    auto propertyVariant = this->property(propertyName.c_str());

    if (!propertyVariant.isNull() && propertyVariant.canConvert<QtObjectHolder *>())
    {
        auto propertyHolder = propertyVariant.value<QtObjectHolder *>();

        if (propertyHolder)
        {
            return std::dynamic_pointer_cast<T>(propertyHolder->getObject());
        }
    }
    return typename T::sptr();
}

}


#endif // IQMLSERVICE_HXX
