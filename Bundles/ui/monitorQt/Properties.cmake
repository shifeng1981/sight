
set( NAME monitorQt )
set( VERSION 0.1 )
set( TYPE BUNDLE )
set( DEPENDENCIES fwCom fwCore fwData fwGui fwGuiQt fwMemory fwRuntime fwServices fwTools gui )
set( REQUIREMENTS  )

set( CONAN_DEPS
    qt/5.11.2@sight/stable
)
if(WIN32 OR APPLE)
    list(APPEND CONAN_DEPS libxml2/2.9.8@sight/stable)
endif()