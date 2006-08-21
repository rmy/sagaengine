!include( ../../../../env/qmake/env.pro ) {
    include( ../../../../env/qmake/missing.pro )
}
message( "[ Application ogre_ui settings ]")

CONFIG += app
TARGET = fly

DESTDIR = .
OBJECTS_DIR = .obj

include( core.pro )
include( client.pro )
include( basic.pro )
include( ogre.pro )
include( ogre_ui.pro )
include( logic.pro )
include( start.pro )

LIBS = -L$${DESTDIR} -lOgreMain -lCEGUIOgreRenderer -lCEGUIBase
INCLUDEPATH = 
!include( proj.pro ) {
    error( "You must link proj.pro to proj_debug.pro or proj_release.pro" )
}

message( Config: $$CONFIG )
message( DestDir: $$DESTDIR )
message( Target: $$TARGET )
message( Template: $$TEMPLATE )
