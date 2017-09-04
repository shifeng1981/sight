set( NAME PoCMultiTexturedMesh )
set( VERSION 0.1 )
set( TYPE APP )
set( DEPENDENCIES  )
set( REQUIREMENTS
    appXml
    fwlauncher

    media

    guiQt

    io
    ioVTK

    uiIO

    visuVTKQt
    visuVTKAdaptor
)

bundleParam(appXml PARAM_LIST config PARAM_VALUES PoCMultiTexturedMeshConfig)
