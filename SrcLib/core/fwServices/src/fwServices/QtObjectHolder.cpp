#include "fwServices/QtObjectHolder.hpp"
#include "fwServices/QtQmlType.hxx"

#include <fwData/Object.hpp>


namespace fwServices
{
static QtQmlType<QtObjectHolder> registar = QtQmlType<::fwServices::QtObjectHolder>("com.fw4spl", 1, 0, "TypeHolder");

QtObjectHolder::QtObjectHolder(std::shared_ptr<::fwData::Object> const& object, QObject *parent) :
    QObject(parent),
    m_object(object)
{
}

QtObjectHolder::QtObjectHolder(QtObjectHolder const& other) : QObject(nullptr)
{
    m_object = other.m_object;
}

QtObjectHolder::QtObjectHolder(QObject *parent): QObject(parent)
{
    m_object = nullptr;
}

::fwData::Object::sptr&    QtObjectHolder::getObject()
{
    return m_object;
}

::fwData::Object::csptr const& QtObjectHolder::getObject() const
{
    return m_object;
}

bool    QtObjectHolder::operator!=(QtObjectHolder const& other)
{
    return other.m_object != m_object;
}

QtObjectHolder&  QtObjectHolder::operator=(QtObjectHolder const& other)
{
    m_object = other.m_object;
    return *this;
}

QtObjectHolder::~QtObjectHolder()
{
}

} // typeReg
