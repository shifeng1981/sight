/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWQTQUICK_UTILS_IO_HPP__
#define __FWQTQUICK_UTILS_IO_HPP__

#include "fwQtQuick/config.hpp"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringList>

namespace fwQtQuick
{
namespace IO
{

namespace FromDialog
{
FWQTQUICK_API const QString SelectSaveFileUrl(const QString& filter, const QString& title = "Save File");
FWQTQUICK_API const QString SelectOpenFileUrl(const QString& filter, const QString& title = "Open File");
} // namespace FromDialog

namespace Read
{
FWQTQUICK_API QJsonObject JsonFromUrl(const QString&);
FWQTQUICK_API QString TextFromUrl(const QString&);
} // namespace Read

namespace Write
{
FWQTQUICK_API void TextToFile(const QString& text, const QString& url);
} // namspace Write

FWQTQUICK_API QStringList FileUrlsFromDir(const QString& dir, QStringList filters);
FWQTQUICK_API bool FileExists(const QString& filePath);

} // namespace IO
} // namespace fwQtQuick

#endif //__FWQTQUICK_UTILS_IO_HPP__
