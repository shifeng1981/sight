fwLoadProperties()

find_package(OpenNI2 QUIET REQUIRED)
find_package(Boost QUIET COMPONENTS program_options filesystem REQUIRED)
find_package(OpenCV QUIET REQUIRED highgui videoio)

fwInclude(
    ${OPENNI2_INCLUDE_DIRS}
    )

fwLink(
    Boost::program_options
    Boost::filesystem
    opencv_highgui
    opencv_videoio
    ${OPENNI2_LIBRARY}
)
