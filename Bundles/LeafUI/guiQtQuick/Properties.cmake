
set( NAME guiQtQuick )
set( VERSION 0.1 )
set( TYPE BUNDLE )
set( START ON )
set( DEPENDENCIES 
    fwActivities
    fwCore
    fwData
    fwDataCamp
    fwGui
    fwGuiQuick
    fwRuntime
    fwServices
    fwTools
    fwQtQuick
)
set( REQUIREMENTS gui )
list(APPEND PLUGINS 
    QtQuick
    platforms
    imageformats

)
