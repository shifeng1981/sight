
set( NAME ioCalibration )
set( VERSION 0.1 )
set( TYPE BUNDLE )
set( DEPENDENCIES
    fwCore
    fwCom
    fwData
    fwRuntime
    fwServices
    fwTools
    arData
    fwCom
    fwGui
    fwIO
    cvIO
)
set( REQUIREMENTS dataReg servicesReg)
set( CONAN_DEPS
    ${CONAN_OPENCV}
)