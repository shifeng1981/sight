
set( NAME Ex05QtQuick )
set( VERSION 0.1 )
set( TYPE APP )
set( DEPENDENCIES
)
set( REQUIREMENTS 
    appXml
    dataReg
    servicesReg
    
    visuVTK
    visuVTKQuick
    visuVTKAdaptor
    visuVTKVRAdaptor
    vtkSimpleMesh

    fwlauncher
    
    io
    ioVTK

    uiIO
    
    gui
    guiQtQuick
)

bundleParam(appXml PARAM_LIST config PARAM_VALUES Ex05QtQuickConfig)
