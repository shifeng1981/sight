if(APPLE)
    include(${FWCMAKE_INSTALL_FILES_DIR}/osx_install.cmake)
elseif(WIN32)
    include(${FWCMAKE_INSTALL_FILES_DIR}/win_install.cmake)
elseif(UNIX)
    #Don't use rpath at install step
    set(CMAKE_SKIP_INSTALL_RPATH ON)
    include(${FWCMAKE_INSTALL_FILES_DIR}/linux_install.cmake)
endif()

macro(generic_install)
    install_qt_plugins()

    if(APPLE)
        osx_install(${FWPROJECT_NAME})
    elseif(WIN32)
        win_install(${FWPROJECT_NAME})
    elseif(UNIX)
        linux_install(${FWPROJECT_NAME})
    endif()

endmacro(generic_install)
