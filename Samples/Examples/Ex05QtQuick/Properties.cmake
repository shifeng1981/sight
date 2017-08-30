
set( NAME Ex05QtQuick )
set( VERSION 0.1 )
set( TYPE APP )
set( DEPENDENCIES
)
set( REQUIREMENTS 
    appXml2
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

bundleParam(appXml2 PARAM_LIST config PARAM_VALUES Ex05QtQuickConfig)
