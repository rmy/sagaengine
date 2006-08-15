include( ../../../../../env/qmake/proj.pro )

message( "[ Application logic settings ]")

CONFIG += lib
TARGET = game_logic

INCLUDEPATH += ../../src

include( src.pro )

message( Config: $$CONFIG )
message( Target: $$TARGET )
message( Template: $$TEMPLATE )

