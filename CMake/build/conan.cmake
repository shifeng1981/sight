# Conan support
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.13/conan.cmake"
                 "${CMAKE_BINARY_DIR}/conan.cmake")
endif()
include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_check(VERSION 1.0.0 REQUIRED)
conan_add_remote(NAME sight INDEX 1
                 URL https://conan.ircad.fr/artifactory/api/conan/sight)

unset(CONAN_OPTIONS)
find_package(CUDA QUIET)
if(CUDA_FOUND)
    message(STATUS "CUDA ${CUDA_VERSION_STRING} found on your PC. Cuda Conan packages will be used")
    set(CONAN_OPTIONS "*:use_cuda=True")
else()
    message(STATUS "CUDA ${CUDA_VERSION_STRING} not found on your PC. Cuda Conan packages will be NOT used")
    set(CONAN_OPTIONS "*:use_cuda=False")
endif()

macro(findConanDeps PROJECT_LIST CONAN_DEPS_LIST)
    unset(RESULT_LIST)

    foreach(PROJECT ${PROJECT_LIST})
        if(${PROJECT}_CONAN_DEPS)
            list(APPEND RESULT_LIST ${${PROJECT}_CONAN_DEPS})
        endif()
    endforeach()

    list(REMOVE_DUPLICATES RESULT_LIST)
    set(CONAN_DEPS_LIST ${RESULT_LIST})
endmacro()

macro(installConanDeps CONAN_DEPS_LIST)

    set(CONAN_BUILD_OPTION "never")
    if(CONAN_BUILD_MISSING)
        set(CONAN_BUILD_OPTION "missing")
    endif()

    if(UNIX AND NOT APPLE)
        # Find information about current Linux distribution to configure conan
        find_program(LSB_RELEASE_EXEC lsb_release)
        if(NOT LSB_RELEASE_EXEC)
            message(FATAL_ERROR "lsb_release executable not found!")
        endif()
        execute_process(COMMAND ${LSB_RELEASE_EXEC} -is
            OUTPUT_VARIABLE LSB_RELEASE_ID_SHORT
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        execute_process(COMMAND ${LSB_RELEASE_EXEC} -rs
            OUTPUT_VARIABLE LSB_RELEASE_NUMBER_SHORT
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        string(TOLOWER ${LSB_RELEASE_ID_SHORT} LSB_RELEASE_ID_SHORT_LOWER)
        set(CONAN_DISTRO "${LSB_RELEASE_ID_SHORT_LOWER}${LSB_RELEASE_NUMBER_SHORT}")
        set(CONAN_SETTINGS "SETTINGS os.distro=${CONAN_DISTRO}")
    endif()

    conan_cmake_run(
        REQUIRES ${CONAN_DEPS_LIST}
        BASIC_SETUP CMAKE_TARGETS NO_OUTPUT_DIRS OUTPUT_QUIET
        OPTIONS ${CONAN_OPTIONS}
        BUILD ${CONAN_BUILD_OPTION}
        ${CONAN_SETTINGS}
    )

endmacro()

macro(installConanDepsForSDK)

    foreach(CONAN_DEP ${CONAN_DEPENDENCIES})
        string(REGEX REPLACE "([^\/]*)/.*" "\\1" CONAN_REQUIREMENT ${CONAN_DEP})
        string(TOUPPER ${CONAN_REQUIREMENT} CONAN_REQUIREMENT )
        install(DIRECTORY "${CONAN_${CONAN_REQUIREMENT}_ROOT}/" DESTINATION ${CMAKE_INSTALL_PREFIX})
    endforeach()

endmacro()
