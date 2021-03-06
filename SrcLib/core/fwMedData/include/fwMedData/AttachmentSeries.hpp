/************************************************************************
 *
 * Copyright (C) 2009-2018 IRCAD France
 * Copyright (C) 2012-2018 IHU Strasbourg
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

#pragma once

#include "fwMedData/config.hpp"
#include "fwMedData/Series.hpp"
#include "fwMedData/types.hpp"

#include <fwData/factory/new.hpp>

#include <fwMemory/BufferObject.hpp>

#include <boost/filesystem/path.hpp>

fwCampAutoDeclareDataMacro((fwMedData)(AttachmentSeries), FWMEDDATA_API);

namespace fwMedData
{

/**
 * @brief Holds attachment file
 * @deprecated This data will be removed in Sight 20.0.
 */
class FWMEDDATA_CLASS_API AttachmentSeries : public ::fwMedData::Series
{

public:
    fwCoreClassDefinitionsWithFactoryMacro( (AttachmentSeries)(::fwData::Object), (()),
                                            ::fwData::factory::New< AttachmentSeries >);

    fwCampMakeFriendDataMacro((fwMedData)(AttachmentSeries));

    /**
     * @brief Constructor
     * @param key Private construction key
     */
    FWMEDDATA_API AttachmentSeries(::fwData::Object::Key key);

    /// Destructor
    FWMEDDATA_API virtual ~AttachmentSeries();

    /// Defines deep copy
    FWMEDDATA_API void cachedDeepCopy( const ::fwData::Object::csptr& _source, DeepCopyCacheType& cache ) override;

    /**
     * @brief Availability
     */
    typedef enum
    {
        NONE     = 1,   /*! The attachment file is not available. */
        LOCAL    = 2,   /*! The attachment file are on the local machine. */
        EMBEDDED = 3    /*! The attachment are saved in this AttachmentSeries. */
    } AttachmentAvailability;

    /**
     * @brief Local path of attachment file
     * @{ */
    FWMEDDATA_API const ::boost::filesystem::path& getAttachmentPath() const;

    FWMEDDATA_API void setAttachmentPath(const ::boost::filesystem::path& path);
    /**  @} */

    /// Returns BufferObject of the current attachment file
    FWMEDDATA_API ::fwMemory::BufferObject::sptr getBufferObject() const;

    /// Returns the state of the attached file
    FWMEDDATA_API AttachmentAvailability getAttachmentAvailability() const;

    /// Returns the media type of the attached file
    FWMEDDATA_API std::string getMediaType() const;

    /// Specifies the media type of the attached file (see IANA list of official media types)
    FWMEDDATA_API void setMediaType(const std::string& mediaType);

protected:

    /// BufferObject of the current attachment file
    ::fwMemory::BufferObject::sptr m_buffer;

    /// Local path of attachment file
    ::boost::filesystem::path m_attachmentPath;

    /// Attachment availability
    AttachmentAvailability m_attachmentAvailability;

    /// Media type of the attached file
    std::string m_mediaType;
};

}   //end namespace fwMedData
