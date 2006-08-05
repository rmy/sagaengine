message( "[ Project settings ]")

CONFIG = debug

message( "core module included" )
INCLUDEPATH += $${SAGAENGINE}/core/include
INCLUDEPATH += $${SAGAENGINE}/core/src

message( "test included" )
INCLUDEPATH += ../../src

DEFINES *= DEBUG_LEVEL_2
