#pragma once

#include "fwServices/config.hpp"
#include "fwServices/macros.hpp"

#include <QObject>

namespace fwServices
{

class FWSERVICES_CLASS_API IQtQmlObject
{
public:
    fwQmlTypeMacro(::fwServices::IQtQmlObject);

	IQtQmlObject(std::string const& cType);
	virtual ~IQtQmlObject();

	/**
	 *	@brief: This method create a new object derived of QObject
     *  This method must be override.
	 */
    virtual QObject *instanciate() const = 0;

	/**
	 *	@brief: This method return the classType
	 */
	std::string const&	getClassType() const;

private:
	std::string	m_cType;
};

} // fwGuiQt
