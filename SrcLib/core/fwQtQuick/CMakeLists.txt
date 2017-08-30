fwLoadProperties()
add_definitions("-DQT_NO_KEYWORDS")
find_package(Qt5 COMPONENTS Core Widgets Qml Quick REQUIRED)
find_package(VTK REQUIRED)

fwForwardInclude(${CMAKE_CURRENT_BINARY_DIR}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Qml_INCLUDE_DIRS}
    ${Qt5Quick_INCLUDE_DIRS}
    ${VTK_INCLUDE_DIRS}
)

fwLink(
    ${Qt5Core_LIBRARIES}
    ${Qt5Qml_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Quick_LIBRARIES}
    ${VTK_LIBRARIES}
)

set_target_properties(${FWPROJECT_NAME} PROPERTIES AUTOMOC TRUE)
