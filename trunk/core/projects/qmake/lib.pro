include( ../../../env/qmake/proj.pro )

message( "[ core settings ]")

CONFIG += staticlib
TARGET = se_core
INCLUDEPATH += ../../src

include( src.pro )

message( Config: $$CONFIG )
message( Target: $$TARGET )
message( Template: $$TEMPLATE )
