include( ../../../../env/qmake/proj.pro )

message( "[ Client add-on settings ]")

CONFIG += staticlib
TARGET = se_client
INCLUDEPATH += ../../src
INCLUDEPATH += ../../../../core/src
DESTDIR = ../../../../lib

message( Target $$TARGET )

include( src.pro )

message( Config: $$CONFIG )
message( Target: $$TARGET )
message( Template: $$TEMPLATE )
