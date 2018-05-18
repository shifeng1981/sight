set( NAME TutoQml3D ) # Name of the application
set( VERSION 0.1 ) # Version of the application
set( TYPE APP ) # Type APP represent "Application"
set( DEPENDENCIES  ) # For an application we have no dependencies (libraries to link)
set( REQUIREMENTS # The bundles used by this application
    gui
    guiQt
    fwlauncher
    fwGuiQt
    appQml
    )

# Set the configuration to use : 'tutoBasicConfig'
bundleParam(appQml PARAM_LIST config PARAM_VALUES tutoBasicConfig) 
