set( NAME PoCMultiTexturedMesh )
set( VERSION 0.1 )
set( TYPE APP )
set( DEPENDENCIES  )
set( REQUIREMENTS
    appXml2
    fwlauncher

    media

    guiQt

    io
    ioVTK

    uiIO

    visuVTKQt
    visuVTKAdaptor
)

bundleParam(appXml2 PARAM_LIST config PARAM_VALUES PoCMultiTexturedMeshConfig)
