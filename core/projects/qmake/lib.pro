message( "[ core settings ]")

CONFIG += lib
OBJECTS_DIR = .obj
TARGET = SagaEngine

DEFINES *= _SeClientExport=

include( core.pro )
include( pc.pro )

INCLUDEPATH = ../../src

!include( ../../../env/qmake/proj.pro ) {
    warning("(SAGAENGINE)/env/qmake/proj.pro is missing")
}



message( Config: $$CONFIG )
message( Target: $$TARGET )
message( Template: $$TEMPLATE )
