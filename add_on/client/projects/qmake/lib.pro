!include( ../../../../env/qmake/proj.pro ) {
    warning("(SAGAENGINE)/env/qmake/proj.pro is missing")
}

message( "[ client add-on settings ]")

CONFIG += lib
OBJECTS_DIR = .obj
TARGET = se_client

DEFINES *= _SeClientExport= _SeCoreExport=

INCLUDEPATH += ../../src
INCLUDEPATH += ../../../../core/src

include( src.pro )

message( Config: $$CONFIG )
message( Target: $$TARGET )
message( Template: $$TEMPLATE )

