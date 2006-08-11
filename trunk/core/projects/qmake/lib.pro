!include( ../../../env/qmake/proj.pro ) {
    warning("(SAGAENGINE)/env/qmake/proj.pro is missing")
}

message( "[ core settings ]")

CONFIG += lib
OBJECTS_DIR = .obj
TARGET = se_core

DEFINES *= _SeOgreExport _SeClientExport _SeCoreExport

include( src.pro )

INCLUDEPATH = ../../src

message( Config: $$CONFIG )
message( Target: $$TARGET )
message( Template: $$TEMPLATE )
