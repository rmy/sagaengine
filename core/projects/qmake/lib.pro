!include( ../../../env/qmake/proj.pro ) {
    warning("(SAGAENGINE)/env/qmake/proj.pro is missing")
}

message( "[ core settings ]")

CONFIG += lib
OBJECTS_DIR = .obj
TARGET = se_core

INCLUDEPATH = ../../src

include( src.pro )

message( Config: $$CONFIG )
message( Target: $$TARGET )
message( Template: $$TEMPLATE )
