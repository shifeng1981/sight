/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWGUIQUICK_UTIL_FUNCSLOT_HPP__
#define __FWGUIQUICK_UTIL_FUNCSLOT_HPP__

#include <fwGuiQuick/config.hpp>

#include <boost/function.hpp>

#include <QObject>

namespace fwGuiQuick
{

namespace util
{

class FWGUIQUICK_CLASS_API FuncSlot : public QObject
{
Q_OBJECT

public:
    FWGUIQUICK_API FuncSlot();

    template< typename CALLABLE >
    FuncSlot(CALLABLE c) :
        m_func(c)
    {
    }

    //------------------------------------------------------------------------------

    template< typename CALLABLE >
    void setFunction(CALLABLE c)
    {
        m_func = c;
    }

public Q_SLOTS:
    void trigger();

protected:
    ::boost::function< void() > m_func;
};

} // namespace util

} // namespace fwGuiQuick

#endif //__FWGUIQUICK_UTIL_FUNCSLOT_HPP__

