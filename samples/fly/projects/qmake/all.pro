!include( ../../../../env/qmake/env.pro ) {
    include( ../../../../env/qmake/missing.pro )
}
message( "[ Application ogre_ui settings ]")

CONFIG += app

DESTDIR = .
OBJECTS_DIR = .obj

# LIBS = -L$${DESTDIR} -lOgreMain -lCEGUIOgreRenderer -lCEGUIBase
LIBS = -lOgreMain -lOIS -L/usr/X11R6/lib -lX11 -lXaw
LIBS += -L/usr/local/src/audiere/src/.libs -laudiere
LIBS += -L/usr/local/lib -laudiere -lvorbisfile -lvorbis -logg -lasound -lpthread

include( core.pro )
include( pc.pro )
include( client.pro )
include( basic.pro )
include( ogre.pro )
include( ogre_ui.pro )
include( logic.pro )
include( start.pro )

INCLUDEPATH = 
!include( proj.pro ) {
    error( "You must link proj.pro to proj_debug.pro or proj_release.pro" )
}
DEFINES *= SE_STATIC
TARGET = fly

message( Config: $$CONFIG )
message( DestDir: $$DESTDIR )
message( Target: $$TARGET )
message( Template: $$TEMPLATE )
