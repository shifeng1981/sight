/************************************************************************
 *
 * Copyright (C) 2009-2017 IRCAD France
 * Copyright (C) 2012-2017 IHU Strasbourg
 *
 * This file is part of Sight.
 *
 * Sight is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Sight. If not, see <https://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#ifndef __FWVTKIO_MESHREADER_HPP__
#define __FWVTKIO_MESHREADER_HPP__

#include "fwVtkIO/config.hpp"

#include <fwData/location/SingleFile.hpp>
#include <fwData/Mesh.hpp>

#include <fwDataIO/reader/GenericObjectReader.hpp>

#include <boost/filesystem/path.hpp>

namespace fwJobs
{
class Observer;
class IJob;
}

namespace fwVtkIO
{

/**
 * @brief   Read a mesh.
 *
 * Read a VTK Mesh using the VTK lib
 */

class MeshReader : public ::fwDataIO::reader::GenericObjectReader< ::fwData::Mesh >,
                   public ::fwData::location::enableSingleFile< ::fwDataIO::reader::IObjectReader >
{

public:

    fwCoreClassDefinitionsWithFactoryMacro((MeshReader)( ::fwDataIO::reader::GenericObjectReader< ::fwData::Mesh >),
                                           (()),
                                           ::fwDataIO::reader::factory::New< MeshReader >
                                           );
    fwCoreAllowSharedFromThis();

    //! @brief Constructor.
    FWVTKIO_API MeshReader(::fwDataIO::reader::IObjectReader::Key key);

    //! @brief Destructor.
    FWVTKIO_API ~MeshReader();

    //! @brief Reading operator.
    FWVTKIO_API void read() override;

    /// @return ".vtk"
    FWVTKIO_API std::string extension() override;
    /// @return internal job
    FWVTKIO_API SPTR(::fwJobs::IJob) getJob() const override;

private:

    ///Internal job
    SPTR(::fwJobs::Observer) m_job;

};

} // namespace fwVtkIO

#endif // __FWVTKIO_MESHREADER_HPP__
