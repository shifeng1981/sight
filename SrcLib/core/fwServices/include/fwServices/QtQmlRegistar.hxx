/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

# include "fwServices/config.hpp"
# include "fwServices/QtQmlObject.hxx"
# include "fwServices/QtQmlInstancier.hxx"

# include <vector>
# include <memory>

namespace fwServices
{

/**
 *  @brief: Registration class
 *      This class a new type based on typename T and cType.
 *      Then store it in QtQmlInstancier
 */
template<typename T>
class QtQmlRegistar
{
public:
    /**
     * @brief: Constructor
     */
    QtQmlRegistar(std::string const& cType)
    {
        std::shared_ptr<QtQmlObject<T> > qtObject(new QtQmlObject<T>(cType));
        QtQmlInstancier::addClass(qtObject);
    }

    /**
     *  @brief: Destructor
     */
    ~QtQmlRegistar()
    {
    }
};

} // fwGuiQt
