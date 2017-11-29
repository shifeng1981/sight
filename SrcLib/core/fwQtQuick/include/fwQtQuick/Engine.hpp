/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWQTQUICK_ENGINE_HPP__
#define __FWQTQUICK_ENGINE_HPP__

#include "fwQtQuick/code/Compiler.hpp"
#include "fwQtQuick/config.hpp"
#include "fwQtQuick/utils/Register.hpp"

#include <boost/filesystem/path.hpp>

#include <QQuickItem>
#include <QQuickView>

namespace fwQtQuick
{

/**
 * @brief   Defines the QML application.
 */
class FWQTQUICK_CLASS_API Engine : public QObject
{
Q_OBJECT
Q_PROPERTY(fwQtQuick::code::Compiler* compiler READ getCompiler CONSTANT)
public:

    FWQTQUICK_API static Engine* getInstance();
    FWQTQUICK_API void init();
    FWQTQUICK_API ::fwQtQuick::code::Compiler* getCompiler();

    //------------------------------------------------------------------------------

    void setFramePath(const ::boost::filesystem::path& framePath)
    {
        m_FramePath = framePath;
    }
    //------------------------------------------------------------------------------

    void setRootPath(const ::boost::filesystem::path& rootPath)
    {
        m_RootPath = rootPath;
    }

    //------------------------------------------------------------------------------

    QObject* getRootObject()
    {
        return m_rootObject;
    }

private:
    Engine();
    virtual ~Engine();

    Engine(Engine const&)         = delete;
    void operator=(Engine const&) = delete;

    static fwQtQuick::Register::Controller<Engine> Register;
    ::boost::filesystem::path m_FramePath;
    ::boost::filesystem::path m_RootPath;
    QObject* m_rootObject;

};

} // namespace fwGuiQt

#endif /*__FWQTQUICK_ENGINE_HPP__*/
