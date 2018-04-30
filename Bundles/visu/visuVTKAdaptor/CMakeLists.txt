fwLoadProperties()

find_package(Boost QUIET COMPONENTS regex REQUIRED)

target_compile_definitions(${FWPROJECT_NAME} PUBLIC "-DQT_NO_KEYWORDS")

find_package(Qt5 QUIET COMPONENTS Core Gui Widgets Concurrent Qml Quick QuickWidgets Sql REQUIRED)

fwForwardInclude(
    ${Boost_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Concurrent_INCLUDE_DIRS}
    ${Qt5Qml_INCLUDE_DIRS}
    ${Qt5Quick_INCLUDE_DIRS}
    ${Qt5QuickWidgets_INCLUDE_DIRS}
    ${Qt5Sql_INCLUDE_DIRS}

)

fwLink(
    ${Boost_REGEX_LIBRARY}
    ${Qt5Core_LIBRARIES}
    ${Qt5Gui_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Concurrent_LIBRARIES}
    ${Qt5Qml_LIBRARIES}
    ${Qt5Quick_LIBRARIES}
    ${Qt5QuickWidgets_LIBRARIES}
    ${Qt5Sql_LIBRARIES}
)

set_target_properties(${FWPROJECT_NAME} PROPERTIES AUTOMOC TRUE)
