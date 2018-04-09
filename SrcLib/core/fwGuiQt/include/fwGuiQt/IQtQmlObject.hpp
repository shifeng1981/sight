#pragma once

#include "fwGuiQt/config.hpp"

#include <QObject>

namespace fwGuiQt
{

class FWGUIQT_CLASS_API IQtQmlObject
{
public:
	IQtQmlObject(std::string const& cType);
	virtual ~IQtQmlObject();

	virtual QObject *instanciate() const = 0;
	std::string const&	getClassType() const;

private:
	std::string	m_cType;
};

} // fwGuiQt