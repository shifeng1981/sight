/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "fwServices/config.hpp"
#include "fwServices/macros.hpp"

#include <QObject>

namespace fwServices
{

class FWSERVICES_CLASS_QT_API IQtQmlObject
{
public:
    fwQmlTypeMacro(::fwServices::IQtQmlObject);

    FWSERVICES_QT_API IQtQmlObject(std::string const& cType);
    FWSERVICES_QT_API virtual ~IQtQmlObject();

    /**
     *  @brief: This method create a new object derived of QObject
     *  This method must be override.
     */
    FWSERVICES_QT_API virtual QObject* instanciate() const = 0;

    /**
     *  @brief: This method return the classType
     */
    FWSERVICES_QT_API std::string const& getClassType() const;

private:
    std::string	m_cType;
};

} // fwGuiQt
