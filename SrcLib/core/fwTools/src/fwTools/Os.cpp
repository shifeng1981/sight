/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include "fwCore/base.hpp"
#include "fwTools/Os.hpp"

#ifdef __linux
#include <sys/stat.h>
#endif

namespace fwTools
{

namespace os
{

//------------------------------------------------------------------------------

std::string getEnv(const std::string& name, bool* ok)
{
    char* value = std::getenv(name.c_str());
    bool exists = (value != NULL);
    if(ok != NULL)
    {
        *ok = exists;
    }
    return std::string(exists ? value : "");
}

//------------------------------------------------------------------------------

std::string getEnv(const std::string& name, const std::string& defaultValue)
{
    bool ok           = false;
    std::string value = getEnv(name, &ok);
    return ok ? value : defaultValue;
}

//------------------------------------------------------------------------------

std::string getUserDataDir( std::string company, std::string appName, bool createDirectory )
{
    std::string dataDir;
#ifdef WIN32
    char* appData = std::getenv("APPDATA");
    dataDir = ::fwTools::os::getEnv("APPDATA");
#else
    bool hasXdgConfigHome     = false;
    bool hasHome              = false;
    std::string xdgConfigHome = ::fwTools::os::getEnv("XDG_CONFIG_HOME", &hasXdgConfigHome);
    std::string home          = ::fwTools::os::getEnv("HOME", &hasHome);
    dataDir = hasXdgConfigHome ? xdgConfigHome : (hasHome ? std::string(home) + "/.config" : "");
#endif

    if ( !company.empty() )
    {
        dataDir += "/" + company;
    }

    if ( !appName.empty() )
    {
        dataDir += "/" + appName;
    }

    if ( !dataDir.empty() )
    {
        if (boost::filesystem::exists(dataDir))
        {
            if ( !boost::filesystem::is_directory(dataDir) )
            {
                OSLM_ERROR( dataDir << " already exists and is not a directory." );
                dataDir = "";
            }
        }
        else if (createDirectory)
        {
            OSLM_INFO("Creating application data directory: "<< dataDir);
            boost::filesystem::create_directories(dataDir);
        }
    }

    return dataDir;
}

//------------------------------------------------------------------------------

std::vector< std::pair< int, std::string > >getDeviceFromVirtualDevice(std::string _devURI)
{

    std::vector< std::pair< int, std::string > > output;
    output.push_back(std::make_pair(-1, "ERROR"));

#ifdef __linux
    // Use stat to get infos on device.
    struct stat sdata;
    int ret = stat(_devURI.c_str(), &sdata);

    if(ret < 0)
    {
        SLM_ERROR("`" + _devURI + "` is not a valid /dev/video# path");
        return output; // Default values with error code.
    }

    std::string subpath;

    if (S_ISBLK(sdata.st_mode))
    {
        subpath = "block/";
    }
    else if (S_ISCHR(sdata.st_mode))
    {
        subpath = "char/";
    }
    else
    {
        SLM_ERROR("`" + _devURI + "` is not a valid /dev/video# path");
        return output; // Default values with error code.
    }

    // Construct a path /sys/dev/{block,char}/<maj>:<min>
    ::boost::filesystem::path path = "/sys/dev/" + subpath + std::to_string(major(sdata.st_rdev)) + ":"
                                     + std::to_string(minor(sdata.st_rdev)) + "/device/../";

    // Follow symlink
    ::boost::filesystem::path realpath = ::boost::filesystem::canonical(path);

    std::ifstream ifs(realpath.string() + "/devnum");
    if(ifs.is_open())
    {
        std::string content( (std::istreambuf_iterator<char>(ifs) ),
                             (std::istreambuf_iterator<char>()    ) );

        // Change default values.
        output[0].first  = std::stoi(content);
        output[0].second = realpath.string();

        ifs.close();
    }
    else
    {
        SLM_ERROR("'devnum' file cannot be read at: '" + realpath.string() + "'. The device number cannot be found for "
                  + _devURI );
        return output; // Default values with error code.
    }

    // Also check associated devnum (for RGBD cam for example).
    // looking for paths like: /sys/devices/pci###/usb3/3.x/3.x.y
    // All 3.x.y belongs to the same usb device.

    // Go to parent directory and check if associated usb device exists. (3.x/)
    ::boost::filesystem::path parentPath = realpath.parent_path();
    ::boost::filesystem::path parentDir  = parentPath.filename();

    // Get last character to exclude it from regex
    // Looking for all directories named 3.x.y except the one already found.
    const ::boost::regex my_filter( parentDir.string() + "\\.[^" + realpath.string().back() + "]" );

    std::vector< std::string > all_matching_files;

    boost::filesystem::directory_iterator end_itr; // Default ctor yields past-the-end
    for( ::boost::filesystem::directory_iterator i( parentPath ); i != end_itr; ++i )
    {
        // Skip if not a directory
        if( !boost::filesystem::is_directory( i->status() ) )
        {
            continue;
        }

        ::boost::smatch what;

        //Skip if don't respect the regex
        if( !::boost::regex_match( i->path().filename().string(), what, my_filter ) )
        {
            continue;
        }

        ::boost::filesystem::path associatedDevPath = i->path();

        std::ifstream ifs(i->path().string() + "/devnum");
        if(ifs.is_open())
        {
            std::string content( (std::istreambuf_iterator<char>(ifs) ),
                                 (std::istreambuf_iterator<char>()    ) );

            output.push_back(std::make_pair(std::stoi(content), i->path().string()));

            ifs.close();
        }
        else
        {
            SLM_ERROR("'devnum' file cannot be read at: '" + associatedDevPath.string()
                      + "'. No associated device were added" );
        }
    }

#endif

    return output;
}

//------------------------------------------------------------------------------

} // namespace os

} // namespace fwTools
