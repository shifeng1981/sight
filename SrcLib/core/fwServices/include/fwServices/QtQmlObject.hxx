/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "fwServices/config.hpp"
#include "fwServices/IQtQmlObject.hpp"

namespace fwServices
{
/**
 *  @brief: This class is a representation of a QMLObject
 *          Type T match with a QObject derived class
 *          Inheritance of IQtQmlObject is important in order to make this class generic
 *          i.e not depend of the type T
 *          Object here are represented in XML files under <ctx>...</ctx>
 */
template<typename T>
class QtQmlObject : public IQtQmlObject
{
public:
    /**
     *  @brief: cType is the string used in xml to instanciate the class
     */
    QtQmlObject(std::string const& cType) :
        IQtQmlObject(cType)
    {
    }

    /**
     * @brief: Destructor, do nothing
     */
    ~QtQmlObject()
    {
    }

    /**
     *  @brief: Instanciate method, look at IQtQmlObject
     */
    QObject* instanciate() const
    {
        return new T();
    }
};
}
