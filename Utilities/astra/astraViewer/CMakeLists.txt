fwLoadProperties()

find_package(OpenNI2 QUIET REQUIRED)
target_link_libraries(${FWPROJECT_NAME} PRIVATE ${OPENNI2_LIBRARIES})

find_package(Boost QUIET REQUIRED COMPONENTS program_options filesystem)
target_link_libraries(${FWPROJECT_NAME} PRIVATE Boost::program_options Boost::filesystem)

find_package(OpenCV QUIET REQUIRED COMPONENTS opencv_videoio opencv_highgui)
target_link_libraries(${FWPROJECT_NAME} PRIVATE opencv_videoio opencv_highgui)

find_package(FFMPEG QUIET REQUIRED)
target_link_libraries(${FWPROJECT_NAME} PRIVATE ${FFMPEG_LIBRARIES})

find_package(Qt5 QUIET COMPONENTS Test REQUIRED)
target_link_libraries(${FWPROJECT_NAME} PRIVATE Qt5::Test)

fwInclude(
    ${OPENNI2_INCLUDE_DIRS}
)
