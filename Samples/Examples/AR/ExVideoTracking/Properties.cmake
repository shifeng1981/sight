
set( NAME ExVideoTracking )
set( VERSION 0.1 )
set( TYPE APP )
set( UNIQUE TRUE)
set( DEPENDENCIES  )
set( REQUIREMENTS
    dataReg
    arDataReg
    gui
    guiQt
    servicesReg
    fwlauncher
    appXml
    visuVTK
    visuVTKQt
    visuVTKAdaptor
    visuVTKARAdaptor
    videoQt
    videoOpenCV
    videoTools
    trackerAruco
    arMedia
    preferences
    )

bundleParam(appXml PARAM_LIST config PARAM_VALUES ExVideoTrackingConfig)
