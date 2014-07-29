/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __GDCMIO_WRITER_SOP_SPATIALFIDUCIALSIOD_HPP__
#define __GDCMIO_WRITER_SOP_SPATIALFIDUCIALSIOD_HPP__

#include "gdcmIO/container/DicomInstance.hpp"
#include "gdcmIO/writer/iod/InformationObjectDefinition.hpp"
#include "gdcmIO/config.hpp"

namespace gdcmIO
{
namespace writer
{
namespace iod
{

/**
 * @class SpatialFiducialsIOD
 * @brief SpatialFiducialsIOD class used to write Spacial Fiducials DICOM files
 */
class GDCMIO_CLASS_API SpatialFiducialsIOD : public ::gdcmIO::writer::iod::InformationObjectDefinition
{

public:
    /**
     * @brief Constructor
     * @param[in] instance DICOM instance used to share information between modules
     */
    GDCMIO_API SpatialFiducialsIOD(SPTR(::gdcmIO::container::DicomInstance) instance,
            ::boost::filesystem::path folderPath);

    /// Destructor
    GDCMIO_API virtual ~SpatialFiducialsIOD();

    /// Write DICOM file
    GDCMIO_API virtual void write(::fwMedData::Series::sptr series);

};

} // namespace iod
} // namespace writer
} // namespace gdcmIO

#endif // __GDCMIO_WRITER_SOP_SPATIALFIDUCIALSIOD_HPP__
