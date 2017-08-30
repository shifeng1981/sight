/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwQtQuick/utils/IO.hpp"

#include <QDirIterator>
#include <QFileDialog>
#include <QTextStream>

namespace fwQtQuick
{
namespace IO
{

//------------------------------------------------------------------------------

namespace FromDialog
{

//------------------------------------------------------------------------------

const QString SelectSaveFileUrl(const QString& filter, const QString& title)
{
    return QFileDialog::getSaveFileName(0, title, 0, filter);
}

//------------------------------------------------------------------------------

const QString SelectOpenFileUrl(const QString& filter, const QString& title)
{
    return QFileDialog::getOpenFileName(0, title, 0, filter);
}

} // namespace FromDialog

//------------------------------------------------------------------------------

namespace Read
{
//------------------------------------------------------------------------------

QJsonObject JsonFromUrl(const QString& url)
{
    QFile file(url);

    if (file.open(QIODevice::ReadOnly))
    {
        return QJsonDocument::fromJson(file.readAll()).object();
    }

    return QJsonObject();
}

//------------------------------------------------------------------------------

QString TextFromUrl(const QString& url)
{
    QString text;
    QFile file(url);

    if (file.exists() && file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        text = file.readAll();
        file.close();
    }

    return text;
}
} // namespace Read

//------------------------------------------------------------------------------

namespace Write
{
//------------------------------------------------------------------------------

void TextToFile(const QString& text, const QString& url)
{
    QFile file(url);

    if (file.exists() && file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        stream << text;
        file.close();
    }
}
} // namespace Write

//------------------------------------------------------------------------------

QStringList FileUrlsFromDir(const QString& dir, QStringList filters)
{
    QDirIterator it(dir, filters, QDir::Files);
    QStringList fileList;

    while (it.hasNext())
    {
        auto path = it.next();
        auto file = it.fileName();

        fileList.append(path);
    }

    return fileList;
}

//------------------------------------------------------------------------------

bool FileExists(const QString& filePath)
{
    QFile file(filePath);
    return file.exists();
}

//------------------------------------------------------------------------------

} //namespace IO
} //namespace fwQtQuick
