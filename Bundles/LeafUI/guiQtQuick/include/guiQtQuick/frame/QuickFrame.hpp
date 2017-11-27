/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __GUIQTQUICK_FRAME_QUICKFRAME_HPP__
#define __GUIQTQUICK_FRAME_QUICKFRAME_HPP__

#include "guiQtQuick/config.hpp"

#include <fwGui/IFrameSrv.hpp>

#include <fwQtQuick/utils/Register.hpp>

#include <fwTools/Failed.hpp>

#include <QObject>

namespace guiQtQuick
{
namespace frame
{

/**
 * @brief   Defines the Quick2 frame for QML application.
 */
class GUIQTQUICK_CLASS_API QuickFrame : public ::fwGui::IFrameSrv,
                                        public QObject
{

public:
    fwCoreServiceClassDefinitionsMacro( (QuickFrame)(::fwGui::IFrameSrv) );

    /// Constructor. Do nothing.
    GUIQTQUICK_API QuickFrame();

    /// Destructor. Do nothing.
    GUIQTQUICK_API virtual ~QuickFrame();

protected:

    /** @name Service methods ( override from ::fwServices::IService )
     * @{
     */

    /**
     * @brief This method is used to configure the class parameters.
     * @see ::fwGui::IFrameSrv::initialize()
     */
    GUIQTQUICK_API virtual void configuring() override;

    /**
     * @brief Register a view with defined id.
     * @see ::fwGui::IFrameSrv::create()
     */
    GUIQTQUICK_API virtual void starting() override;

    /**
     * @brief This method remove the view in the frame.
     * @see ::fwGui::IFrameSrv::destroy()
     */
    GUIQTQUICK_API virtual void stopping() override;

    /**
     * @brief This method is used to update services. Do nothing.
     */
    GUIQTQUICK_API virtual void updating() override;

    ///@}

    std::string m_framePath;
};

} // namespace frame
} // namespace gui

#endif /*__GUIQTQUICK_FRAME_QUICKFRAME_HPP__*/
