
set( NAME Tuto16MultithreadConsole )
set( VERSION 0.1 )
set( TYPE APP )
set( DEPENDENCIES
    fwDataTools
    fwServices
)
set( REQUIREMENTS
    dataReg
    servicesReg
    fwlauncher
    appXml2
    # The bundle console is used to run a console application with a main loop
    # Typically it can be used for server command line applications
    console
)

bundleParam(appXml2 PARAM_LIST config PARAM_VALUES Tuto16MultithreadConsoleConfig)