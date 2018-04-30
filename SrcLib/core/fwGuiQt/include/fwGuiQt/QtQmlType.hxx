#pragma once

# include "fwGuiQt/config.hpp"

#include <QQmlEngine>

#include <iostream>

namespace fwGuiQt
{

/**
 *	@brief: QtQmlType represent a Type in QML like a Button
 *		These type aren't define in .xml file.
 *
 */
template<typename Type>
class FWGUIQT_CLASS_API QtQmlType
{
public:
	/**
	 *	@brief: constructor, call qmlRegisterType<Type> that precise to QML which class is used for this typename
	 */
	FWGUIQT_API QtQmlType(std::string const& packageName, int versionMajor, int versionMinor, std::string const& objectName)
	{
		qmlRegisterType<Type>(packageName.c_str(), versionMajor, versionMinor, objectName.c_str());
	}

	FWGUIQT_API QtQmlType(std::string const& typeName)
    	{
        	qmlRegisterInterface<Type>(typeName.c_str());
	}

	/**
	 *	@brief: destructor, do nothing.
	 */
	FWGUIQT_API ~QtQmlType()
	{
	}

};

} // fwGuiQt
