!include( ../../../../env/qmake/proj.pro ) {
    warning("(SAGAENGINE)/env/qmake/proj.pro is missing")
}

message( "[ client add-on settings ]")

CONFIG += staticlib
OBJECTS_DIR = .obj
TARGET = se_client

INCLUDEPATH += ../../src
INCLUDEPATH += ../../../../core/src

include( src.pro )

message( Config: $$CONFIG )
message( Target: $$TARGET )
message( Template: $$TEMPLATE )

