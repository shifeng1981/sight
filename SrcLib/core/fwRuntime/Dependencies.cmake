find_package(Boost QUIET COMPONENTS regex REQUIRED)
find_package(LibXml2 QUIET REQUIRED)
if(WIN32)
    find_package(iconv QUIET REQUIRED)
endif()
