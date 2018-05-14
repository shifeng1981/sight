#include "fwServices/IQtQmlObject.hpp"

namespace fwServices
{

IQtQmlObject::IQtQmlObject(std::string const& cType)
{
	m_cType = cType;
}

IQtQmlObject::~IQtQmlObject()
{
}

std::string const&	IQtQmlObject::getClassType() const
{
	return m_cType;
}

} // fwGuiQt
