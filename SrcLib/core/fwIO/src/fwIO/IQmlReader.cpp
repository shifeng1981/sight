/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwIO/IQmlReader.hpp"

#include <fwRuntime/operations.hpp>

#include <iostream>

using fwRuntime::ConfigurationElementContainer;

namespace fwIO
{

IQmlReader::IQmlReader() noexcept
{
    //QObject::connect(this, SIGNAL(filepathChanged(QString const&)), this, SLOT(configure()));
}

//-----------------------------------------------------------------------------

IQmlReader::~IQmlReader() noexcept
{
}

//-----------------------------------------------------------------------------

std::string IQmlReader::getSelectorDialogTitle()
{
    return "Choose a file";
}

//-----------------------------------------------------------------------------

std::vector< std::string > IQmlReader::getSupportedExtensions()
{
    return std::vector< std::string >();
}

//-----------------------------------------------------------------------------

const ::boost::filesystem::path& IQmlReader::getFile() const
{
    FW_RAISE_IF("This reader doesn't manage files", !(this->getIOPathType() & ::fwIO::FILE));
    FW_RAISE_IF("Exactly one file must be defined in location", m_locations.size() != 1);
    return m_locations.front();
}

//-----------------------------------------------------------------------------

void IQmlReader::setFile( const ::boost::filesystem::path& file)
{
    FW_RAISE_IF("This reader doesn't manage files", !(this->getIOPathType() & ::fwIO::FILE));
    m_locations.clear();
    m_locations.push_back(file);
}

//-----------------------------------------------------------------------------

const ::fwIO::LocationsType& IQmlReader::getFiles() const
{
    FW_RAISE_IF("This reader doesn't manage files", !(this->getIOPathType() & ::fwIO::FILES));
    FW_RAISE_IF("At least one file must be define in location", m_locations.empty() );
    return m_locations;
}

//-----------------------------------------------------------------------------

void IQmlReader::setFiles(const ::fwIO::LocationsType& files)
{
    FW_RAISE_IF("This reader doesn't manage files", !(this->getIOPathType() & ::fwIO::FILES));
    m_locations = files;
}

//-----------------------------------------------------------------------------

const ::boost::filesystem::path& IQmlReader::getFolder() const
{
    FW_RAISE_IF("This reader doesn't manage folders", !(this->getIOPathType() & ::fwIO::FOLDER));
    FW_RAISE_IF("Exactly one folder must be define in location", m_locations.size() != 1 );
    return m_locations.front();
}

//-----------------------------------------------------------------------------

void IQmlReader::setFolder(const ::boost::filesystem::path& folder)
{
    FW_RAISE_IF("This reader doesn't manage folders", !(this->getIOPathType() & ::fwIO::FOLDER));
    m_locations.clear();
    m_locations.push_back(folder);
}

//-----------------------------------------------------------------------------

void IQmlReader::setFileFolder(boost::filesystem::path folder)
{
    FW_RAISE_IF("This reader doesn't manage file or files",
                !(this->getIOPathType() & ::fwIO::FILE) && !(this->getIOPathType() & ::fwIO::FILES));

    for(auto& file : m_locations)
    {
        file = file.filename();
        file = folder / file;
    }
}

//-----------------------------------------------------------------------------

const ::fwIO::LocationsType& IQmlReader::getLocations() const
{
    FW_RAISE_IF("At least one path must be define in location", m_locations.empty() );
    return m_locations;
}

//-----------------------------------------------------------------------------

void IQmlReader::clearLocations()
{
    m_locations.clear();
}

//-----------------------------------------------------------------------------

void IQmlReader::configuring()
{
    SLM_ASSERT("Generic configuring method is only available for io service that use paths.",
               !( this->getIOPathType() & ::fwIO::TYPE_NOT_DEFINED ) );

    /*SLM_ASSERT("This reader does not manage folders and a folder path is given in the configuration",
               ( this->getIOPathType() & ::fwIO::FOLDER ) || (config.count("folder") == 0) );*/

    SLM_ASSERT("This reader does not manage files and a file path is given in the configuration",
               ( this->getIOPathType() & ::fwIO::FILE || this->getIOPathType() & ::fwIO::FILES ) ||
               (m_filepath.count() == 0));


    if ( this->getIOPathType() & ::fwIO::FILE )
    {
        FW_RAISE_IF("This reader cannot manages FILE and FILES.", this->getIOPathType() & ::fwIO::FILES );
        FW_RAISE_IF("At least one file must be defined in configuration", m_filepath.count() == 0 );
//        if (config.count("file") == 1)
//        {
//            const std::string file = config.get<std::string>("file");
            this->setFile(::boost::filesystem::path(m_filepath.toStdString()));
//        }
/*        else if (config.count("resource") == 1)
        {
            const std::string resource = config.get<std::string>("resource");
            const auto file            = ::fwRuntime::getResourceFilePath(resource);
            this->setFile(file);
        }*/
    }

   /* if ( this->getIOPathType() & ::fwIO::FILES )
    {
        FW_RAISE_IF("This reader cannot manage FILE and FILES.", this->getIOPathType() & ::fwIO::FILE );

        ::fwIO::LocationsType locations;

        const auto filesCfg = config.equal_range("file");
        for (auto fileCfg = filesCfg.first; fileCfg != filesCfg.second; ++fileCfg)
        {
            const std::string location = fileCfg->second.get_value<std::string>();
            locations.push_back(::boost::filesystem::path(location));
        }

        const auto resourcesCfg = config.equal_range("resource");
        for (auto resourceCfg = resourcesCfg.first; resourceCfg != resourcesCfg.second; ++resourceCfg)
        {
            const std::string resource = resourceCfg->second.get_value<std::string>();
            const auto file            = ::fwRuntime::getResourceFilePath(resource);
            locations.push_back(file);
        }
        this->setFiles(locations);
    }

    if ( this->getIOPathType() & ::fwIO::FOLDER )
    {
        FW_RAISE_IF("At least one folder must be defined in configuration", config.count("folder") > 1 );
        if (config.count("folder") == 1)
        {
            const std::string folder = config.get<std::string>("folder");
            this->setFolder(::boost::filesystem::path(folder));
        }
        else if (config.count("resource") == 1)
        {
            const std::string resource = config.get<std::string>("resource");
            auto folder                = ::fwRuntime::getBundleResourceFilePath(resource);
            if(folder.empty())
            {
                // If not found in a bundle, look into libraries
                folder = ::fwRuntime::getLibraryResourceFilePath(resource);
                SLM_ERROR_IF("Resource '" + resource + "' has not been found in any bundle or library", folder.empty());
            }

            this->setFolder(folder);
        }
    }*/
}

//-----------------------------------------------------------------------------

::fwIO::IOPathType IQmlReader::getIOPathType() const
{
    return ::fwIO::TYPE_NOT_DEFINED;
}

//-----------------------------------------------------------------------------

bool IQmlReader::hasLocationDefined() const
{
    return m_locations.size() > 0;
}

//-----------------------------------------------------------------------------

void IQmlReader::readFolder(::boost::filesystem::path folder)
{
    this->setFolder(folder);
    this->updating();
}

//-----------------------------------------------------------------------------

void IQmlReader::readFile(::boost::filesystem::path file)
{
    this->setFile(file);
    this->updating();
}

//-----------------------------------------------------------------------------

void IQmlReader::readFiles(::fwIO::LocationsType files)
{
    this->setFiles(files);
    this->updating();
}

//-----------------------------------------------------------------------------

}
