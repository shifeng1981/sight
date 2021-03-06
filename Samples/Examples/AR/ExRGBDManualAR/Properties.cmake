
set( NAME ExRGBDManualAR )
set( VERSION 0.1 )
set( TYPE APP )
set( DEPENDENCIES  )
set( REQUIREMENTS
    appXml
    arDataReg
    arMedia
    ctrlCamp
    dataReg
    fwlauncher
    gui
    guiQt
    ioCalibration
    uiPreferences
    uiTools
    maths
    preferences
    servicesReg
    videoQt
    videoTools
    visuVTK
    visuVTKAdaptor
    visuVTKARAdaptor
    visuVTKQt
    fwVtkIO
    ioData
    uiIO
    ioVTK
    ioAtoms
    uiMedDataQt
    dataManagerConfig
    colourSegmentation

    )

bundleParam(appXml PARAM_LIST config PARAM_VALUES ExRGBDManualARConfig)
