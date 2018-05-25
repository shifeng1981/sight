#pragma once

#include "fwServices/config.hpp"
#include "fwServices/IQtQmlType.hpp"

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
class QtQmlType: public IQtQmlType
{
public:
    /**
     *	@brief: constructor, call qmlRegisterType<Type> that precise to QML which class is used for this typename
     */
    QtQmlType(std::string const& packageName, int versionMajor, int versionMinor, std::string const& objectName)
    {
        m_packageName = packageName;
        m_versionMajor = versionMajor;
        m_versionMinor = versionMinor;
        m_objectName = objectName;
    }

    void    registar() const
    {
        std::cout << "Registar : " << m_packageName.c_str() << " " << m_versionMajor << " " << m_versionMinor << " " << m_objectName << std::endl;
        qmlRegisterType<Type>(m_packageName.c_str(), m_versionMajor, m_versionMinor, m_objectName.c_str());
    }

    /**
     *	@brief: destructor, do nothing.
     */
    ~QtQmlType()
    {
    }

private:
    std::string m_packageName = "";
    int m_versionMajor = -1;
    int m_versionMinor = -1;
    std::string m_objectName = "";

};

} // fwServices
