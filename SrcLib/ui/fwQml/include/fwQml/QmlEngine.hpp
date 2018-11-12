/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "fwQml/config.hpp"

#include <fwCore/macros.hpp>

#include <boost/filesystem/path.hpp>

#include <QList>
#include <QPointer>
#include <QQmlApplicationEngine>
#include <QQmlContext>

namespace fwQml
{

/**
 * @brief This class allows to manage the QQmlEngine
 *
 * It allows to define the components to load and the main component to launch.
 */
class QmlEngine
{
public:

    QmlEngine();

    ~QmlEngine();

    /// Return the QmlEngine singleton
    FWQML_API static SPTR(QmlEngine) getDefault();

    /**
     * @brief Load and launch the qml component as the root component
     * @see Use getRootObjects() to get the main objects.
     */
    FWQML_API void loadMainComponent(const ::boost::filesystem::path& file);

    /**
     * @brief Define the path as a directory where the engine searches for installed modules in a URL-based directory
     * structure.
     *
     * The directory should contain a sub directory with the qml files and a qmldir file that describe the module.
     *
     * Example:
     * - declare your qm files in the rc directory of your bundle in a subdirectory named as your bundle:
     * @verbatim
        - myBundle
           - include
           - rc
              - myBundle
                 - qmldir
                 - file1.qml
                 - file2.qml
           - src
        @endverbatim
     * -  import the path using:
     * @code{.cpp}
         SPTR(::fwQml::QmlEngine) engine = ::fwQml::QmlEngine::getDefault();
         engine->importModulePath(::fwRuntime::getBundleResourcePath("myBundle"));
       @endcode
     *
     * @see http://doc.qt.io/qt-5/qtqml-syntax-directoryimports.html
     */
    FWQML_API void importModulePath(const ::boost::filesystem::path& path);

    /// Returns a list of all the root objects instantiated by the QQmlApplicationEngine
    FWQML_API QList<QObject*> getRootObjects();

    /**
     * @brief Returns the engine's root context.
     *
     * The root context is automatically created by the QQmlEngine. Data that should be available to all QML component
     * instances instantiated by the engine should be put in the root context.
     * Additional data that should only be available to a subset of component instances should be added to sub-contexts
     * parented to the root context.
     */
    FWQML_API QQmlContext* getRootContext();

private:

    /// Engine singleton
    static SPTR(QmlEngine) s_current;

    /// qml engine
    QPointer<QQmlApplicationEngine> m_engine;

};

} // fwQml
