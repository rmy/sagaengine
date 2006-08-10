!include( ../../../../env/qmake/env.pro ) {
    include( ../../../../env/qmake/missing.pro )
}
!include( ../../../../env/qmake/proj.pro ) {
    warning("(SAGAENGINE)/env/qmake/proj.pro is missing")
}

message( "[ ogre add-on settings ]")

CONFIG += lib
OBJECTS_DIR = .obj
TARGET = se_ogre

INCLUDEPATH += ../../src

message( "core module included" )
INCLUDEPATH += ../../../../core/include
INCLUDEPATH += ../../../../core/src

message( "client add-on included" )
INCLUDEPATH += ../../../client/include
INCLUDEPATH += ../../../client/src

message( "CEGUI included" )
INCLUDEPATH += $${CEGUI}
message( "OGRE included" )
INCLUDEPATH += $${OGRE}

DEFINES *= IS_OGRE

include( src.pro )

message( Config: $$CONFIG )
message( Target: $$TARGET )
message( Template: $$TEMPLATE )
