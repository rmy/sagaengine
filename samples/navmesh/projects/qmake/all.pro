!include( ../../../../env/qmake/env.pro ) {
    include( ../../../../env/qmake/missing.pro )
}
!include( proj.pro ) {
    error( "You must link proj.pro to proj_debug.pro or proj_release.pro" )
}

message( "[ Application ogre_ui settings ]")

CONFIG += app
TARGET = navmesh

LIBS = -lOgreMain -lCEGUIOgreRenderer -lCEGUIBase -langelscript

DESTDIR = .
OBJECTS_DIR = .obj

include( core.pro )
include( pc.pro )
include( basic.pro )
include( client.pro )
include( ogre.pro )
include( ogre_ui.pro )
include( logic.pro )
include( start.pro )

message( Config: $$CONFIG )
message( DestDir: $$DESTDIR )
message( Target: $$TARGET )
message( Template: $$TEMPLATE )
