include( ../../../../../env/qmake/proj.pro )

message( "[ Application ogre_ui settings ]")

CONFIG += lib
TARGET = ui

INCLUDEPATH += ../../src

include( src.pro )

message( Config: $$CONFIG )
message( Target: $$TARGET )
message( Template: $$TEMPLATE )

