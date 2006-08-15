include( ../../../../../env/qmake/proj.pro )

message( "[ Application ogre_ui settings ]")

CONFIG += lib
TARGET = game_ogre_ui

INCLUDEPATH += ../../src

include( src.pro )

message( Config: $$CONFIG )
message( Target: $$TARGET )
message( Template: $$TEMPLATE )

