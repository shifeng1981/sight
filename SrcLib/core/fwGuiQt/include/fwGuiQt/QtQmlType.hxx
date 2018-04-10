#pragma once

# include "fwGuiQt/config.hpp"

#include <QQmlEngine>

namespace fwGuiQt
{

template<typename Type>
class FWGUIQT_CLASS_API QtQmlType
{
public:
	FWGUIQT_API QtQmlType(std::string const& packageName, int versionMajor, int versionMinor, std::string const& objectName)
	{
		qmlRegisterType<Type>(packageName.c_str(), versionMajor, versionMinor, objectName.c_str());
	}

	FWGUIQT_API ~QtQmlType()
	{
	}
	
};

} // fwGuiQt