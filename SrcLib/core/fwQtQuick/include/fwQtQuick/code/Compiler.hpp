/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWQTQUICK_CODE_COMPILER_HPP__
#define __FWQTQUICK_CODE_COMPILER_HPP__

#include "fwQtQuick/config.hpp"
#include "fwQtQuick/utils/Register.hpp"

#include <QObject>

namespace fwQtQuick
{
namespace code
{

class FWQTQUICK_CLASS_API Compiler : public QObject
{
Q_OBJECT
Q_PROPERTY(QString source
           READ getSource
           NOTIFY sourceChanged
           )
Q_PROPERTY(bool failed
           READ getFailed
           WRITE setFailed
           NOTIFY failedChanged
           )
private:
    static ::fwQtQuick::Register::Controller<Compiler> Register;
    QString m_source;
    bool m_failed;
    Compiler();

public:
    FWQTQUICK_API static Compiler* instance;
    FWQTQUICK_API static void Create();
    FWQTQUICK_API void setSource(const QString&);
    FWQTQUICK_API QString getSource();
    FWQTQUICK_API void setFailed(bool);
    FWQTQUICK_API bool getFailed();
    FWQTQUICK_API void openFile(QString filePath);

Q_SIGNALS:
    void sourceChanged();
    void failedChanged();
};
} // namespace render
} // namespace fwQtQuick

#endif // __FWQTQUICK_CODE_COMPILER_HPP__
