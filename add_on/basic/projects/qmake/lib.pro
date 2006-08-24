!include( ../../../../env/qmake/proj.pro ) {
    warning("(SAGAENGINE)/env/qmake/proj.pro is missing")
}

message( "[ basic add-on settings ]")

CONFIG += lib
OBJECTS_DIR = .obj
TARGET = se_basic

DEFINES *=  _SeBasicExport= _SeClientExport= _SeCoreExport=

INCLUDEPATH += ../../src
INCLUDEPATH += ../../../client/src
INCLUDEPATH += ../../../../core/src

include( src.pro )

message( Config: $$CONFIG )
message( Target: $$TARGET )
message( Template: $$TEMPLATE )

