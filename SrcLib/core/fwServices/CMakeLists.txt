fwLoadProperties()

find_package(Boost QUIET COMPONENTS regex REQUIRED)
if(WIN32)
    set(FW_QT5_PLUGINS_PATH "bin/qt5/plugins")
else()
    set(FW_QT5_PLUGINS_PATH "lib/qt5/plugins")
endif()

configure_file("${CMAKE_CURRENT_LIST_DIR}/bin/qt.conf" "${CMAKE_BINARY_DIR}/bin/qt.conf")

target_compile_definitions(${FWPROJECT_NAME} PUBLIC "-DQT_NO_KEYWORDS")

find_package(Qt5 QUIET COMPONENTS Core Concurrent Qml Quick QuickControls2 REQUIRED)

fwForwardInclude(
    ${Boost_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Concurrent_INCLUDE_DIRS}
    ${Qt5Qml_INCLUDE_DIRS}
    ${Qt5Quick_INCLUDE_DIRS}
    ${Qt5QuickControls2_INCLUDE_DIRS}
)

fwLink(
    ${Boost_REGEX_LIBRARY}
    ${Qt5Core_LIBRARIES}
    ${Qt5Gui_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Concurrent_LIBRARIES}
    ${Qt5Qml_LIBRARIES}
    ${Qt5Quick_LIBRARIES}
    ${Qt5QuickControls2_LIBRARIES}
)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set_target_properties(${FWPROJECT_NAME} PROPERTIES AUTOMOC TRUE)
