/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "fwServices/config.hpp"
#include "fwServices/macros.hpp"

#include <vector>

namespace fwServices
{
class FWSERVICES_CLASS_API IQtQmlType
{
public:
    using ClassList = std::vector<IQtQmlType*>;

    FWSERVICES_API IQtQmlType();
    FWSERVICES_API ~IQtQmlType();

    static FWSERVICES_API void registarAllClasses();

protected:
    virtual FWSERVICES_API void registar() const = 0;

private:
    static FWSERVICES_API ClassList m_classList;
};

} // IQtQmlType
