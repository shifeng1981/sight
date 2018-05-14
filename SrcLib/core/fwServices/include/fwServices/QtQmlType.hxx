#pragma once

# include "fwServices/config.hpp"

#include <QQmlEngine>

#include <iostream>

namespace fwServices
{

/**
 *	@brief: QtQmlType represent a Type in QML like a Button
 *		These type aren't define in .xml file.
 *
 */
template<typename Type>
class FWSERVICES_CLASS_API QtQmlType
{
public:
	/**
	 *	@brief: constructor, call qmlRegisterType<Type> that precise to QML which class is used for this typename
	 */
    FWSERVICES_API QtQmlType(std::string const& packageName, int versionMajor, int versionMinor, std::string const& objectName)
	{
        std::cout << "Register : " << packageName << std::endl;
		qmlRegisterType<Type>(packageName.c_str(), versionMajor, versionMinor, objectName.c_str());
	}

    FWSERVICES_API QtQmlType(std::string const& typeName)
    	{
        	qmlRegisterInterface<Type>(typeName.c_str());
	}

	/**
	 *	@brief: destructor, do nothing.
	 */
    FWSERVICES_API ~QtQmlType()
	{
	}

};

} // fwGuiQt
