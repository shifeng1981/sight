/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once


#include "ioVTK/config.hpp"  // Declaration of class and function export

#include <fwData/Image.hpp>

#include <fwIO/IQmlReader.hpp> // Definition of abstract reader class

#include <fwGuiQt/QtObjectHolder.hpp>

#include <boost/filesystem/path.hpp> // Used to save the file system path of loaded image


namespace fwJobs
{
class IJob;
}

namespace ioVTK
{

/**
 * @brief VTK Image Reader
 *
 * Service reading a VTK Image using the fwVtkIO lib.
 */
class IOVTK_CLASS_API SQImageReader : public ::fwIO::IQmlReader
{
    Q_OBJECT

    Q_PROPERTY(fwGuiQt::QtObjectHolder* image MEMBER m_image NOTIFY imageChanged)


public:
    ~SQImageReader() noexcept
    {
    }

//    typedef ::fwCom::Signal< void ( SPTR(::fwJobs::IJob) ) > JobCreatedSignalType;

    /**
     * @brief Configure the image path with a dialogBox.
     *
     * This method is used to find the file path using a file selector.
     */
    IOVTK_API virtual void configureWithIHM() override;

    /**
     * @brief Constructor. Do nothing.
     */
    IOVTK_API SQImageReader() noexcept;

protected:

    IOVTK_API virtual ::fwIO::IOPathType getIOPathType() const override;

    /// Method called when the service is started, does nothing.
    IOVTK_API virtual void starting() override;

    /// Method called when the service is stopped, does nothing.
    IOVTK_API virtual void stopping() override;

    /// Method called when the service is configured.
    IOVTK_API virtual void configuring() override;

    /// Method called when the service is reconfigured.
    IOVTK_API virtual void reconfiguring() override;

    /// Method called when the service is destroyed, does nothing.
    IOVTK_API virtual void destroying() override;

    /**
     * @brief Updating method execute the read process.
     *
     * This method is used to update the service.
     * The image is read.
     */
    IOVTK_API void updating() override;

Q_SIGNALS:
    void    imageChanged(::fwGuiQt::QtObjectHolder*);


public:
    ::fwGuiQt::QtObjectHolder   *object() const {
        return m_image;
    }

private:

    /**
     * @brief This method is used to load an vtk image using a file path.
     * @param[in] _vtkFile file system path of vtk image
     * @param[out] _pImage new empty image that will contain image loaded, if reading process is a success.
     * @return bool  \b true if the image loading is a success and \b false if it fails
     */
    bool loadImage( const ::boost::filesystem::path _vtkFile, std::shared_ptr<::fwData::Image> _pImage );

    /// This method notifies other image services that a new image has been loaded.
    void notificationOfDBUpdate();

    /// This value is \b true if the path image is known.
    bool m_bServiceIsConfigured;

    /// Image path, location of image on filesystem.
    ::boost::filesystem::path m_fsImgPath;
    //SPTR(JobCreatedSignalType) m_sigJobCreated;
    ::fwGuiQt::QtObjectHolder *m_image;
};

} // namespace ioVTK
