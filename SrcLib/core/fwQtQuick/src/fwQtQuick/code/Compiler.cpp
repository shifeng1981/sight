/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwQtQuick/code/Compiler.hpp"

#include "fwQtQuick/utils/IO.hpp"

namespace fwQtQuick
{

namespace code
{

//------------------------------------------------------------------------------

Compiler* Compiler::instance = nullptr;

::fwQtQuick::Register::Controller<Compiler> Compiler::Register;

//------------------------------------------------------------------------------

Compiler::Compiler()
{
    if (instance)
    {
        throw std::runtime_error("instance already existing");
    }

    this->m_failed = false;
    instance       = this;
}

//------------------------------------------------------------------------------

void Compiler::Create()
{
    instance = new Compiler();
}

//------------------------------------------------------------------------------

void Compiler::setFailed(bool failed)
{
    if (this->m_failed != failed)
    {
        this->m_failed = failed;
        Q_EMIT this->failedChanged();
    }
}

//------------------------------------------------------------------------------

bool Compiler::getFailed()
{
    return this->m_failed;
}

//------------------------------------------------------------------------------

void Compiler::setSource(const QString& source)
{
    this->m_source = source;

    Q_EMIT this->sourceChanged();
}

//------------------------------------------------------------------------------

QString Compiler::getSource()
{
    return this->m_source;
}

//------------------------------------------------------------------------------

void Compiler::openFile(QString filePath)
{
    if (IO::FileExists(filePath))
    {
        this->setSource(IO::Read::TextFromUrl(filePath));
    }
}

//------------------------------------------------------------------------------

} // namespace code
} // namespace fwQtQuick
