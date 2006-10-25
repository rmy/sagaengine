!include( ../../../../../env/qmake/env.pro ) {
    include( ../../../../../env/qmake/missing.pro )
}
message( "[ Application ogre_ui settings ]")

CONFIG *= app

include( core.pro )
include( platform_pc.pro )
include( start.pro )

DESTDIR = .
OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .rcc
UI_DIR = .ui
TARGET = assetEditor

LIBS *= -lOgreMain

unix {
    # You may need to change this include directory
    INCLUDEPATH += /usr/local/include/OGRE /usr/include/OGRE
    CONFIG += link_pkgconfig
    PKGCONFIG += OGRE
} else {
    RC_FILE = resources.rc
    Release:LIBPATH += C:\OgreSDK\bin\release
    Debug:LIBPATH += C:\OgreSDK\bin\debug
    INCLUDEPATH += C:\OgreSDK\include
}



DEPENDPATH = 
INCLUDEPATH = 
!include( proj.pro ) {
    error( "You must link proj.pro to proj_debug.pro or proj_release.pro" )
}

message( Config: $$CONFIG )
message( DestDir: $$DESTDIR )
message( Target: $$TARGET )
message( Template: $$TEMPLATE )
message( $$INCLUDEPATH )

