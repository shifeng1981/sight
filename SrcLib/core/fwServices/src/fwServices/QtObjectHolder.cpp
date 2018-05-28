/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwServices/QtObjectHolder.hpp"

#include "fwServices/QtQmlType.hxx"

#include <fwData/factory/new.hpp>
#include <fwData/Object.hpp>

namespace fwServices
{
static QtQmlType<QtObjectHolder> registar = QtQmlType<::fwServices::QtObjectHolder>("com.fw4spl", 1, 0, "TypeHolder");

QtObjectHolder::QtObjectHolder(std::shared_ptr<::fwData::Object> const& object, QObject* parent) :
    QObject(parent),
    m_object(object)
{
}

QtObjectHolder::QtObjectHolder(QtObjectHolder const& other) :
    QObject(nullptr)
{
    m_object = other.m_object;
}

QtObjectHolder::QtObjectHolder(QObject* parent) :
    QObject(parent)
{
    m_object = nullptr;
    QObject::connect(this, SIGNAL(objectTypeChanged(QString const&)), this, SLOT(create()));
}

//------------------------------------------------------------------------------

::fwData::Object::sptr&    QtObjectHolder::getObject()
{
    return m_object;
}

//------------------------------------------------------------------------------

const ::fwData::Object::csptr QtObjectHolder::getObject() const
{
    return m_object;
}

//------------------------------------------------------------------------------

bool QtObjectHolder::operator!=(QtObjectHolder const& other)
{
    return other.m_object != m_object;
}

//------------------------------------------------------------------------------

QtObjectHolder&  QtObjectHolder::operator=(QtObjectHolder const& other)
{
    m_object = other.m_object;
    return *this;
}

QtObjectHolder::~QtObjectHolder()
{
}

//------------------------------------------------------------------------------

void QtObjectHolder::create()
{
    SLM_ASSERT("Property objectType must be initialize before call to create()", !m_objectType.isEmpty());
    m_object = ::fwData::factory::New(m_objectType.toStdString());
}

} // typeReg
