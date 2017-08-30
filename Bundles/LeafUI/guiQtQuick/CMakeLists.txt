fwLoadProperties()

add_definitions(-DQT_NO_KEYWORDS)

if(${FWPROJECT_NAME}_INSTALL)
    install(DIRECTORY "${QT_DIR}/qml/" DESTINATION qml)
endif()

find_package(Boost REQUIRED)
find_package(Qt5 COMPONENTS Core Gui Widgets Qml Quick REQUIRED)

fwForwardInclude(
    ${Boost_INCLUDE_DIRS} 
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${QtQml_INCLUDE_DIRS}
    ${Qt5Quick_INCLUDE_DIRS}
    )
fwLink(
    ${Boost_REGEX_LIBRARY}
    ${Qt5Core_LIBRARIES}
    ${Qt5Gui_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Qml_LIBRARIES}
    ${Qt5Quick_LIBRARIES}
)

set_target_properties(${FWPROJECT_NAME} PROPERTIES AUTOMOC TRUE)


