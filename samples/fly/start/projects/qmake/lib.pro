include( ../../../../../env/qmake/proj.pro )

message( "[ Application ogre_ui settings ]")

CONFIG += app
TARGET = fly

LIBS = -lgame_logic -lgame_ogre_ui -lse_ogre -lse_client -lse_core -L$${DESTDIR} -lOgreMain -lCEGUIOgreRenderer -lCEGUIBase

INCLUDEPATH += ../../src


include( src.pro )

message( Config: $$CONFIG )
message( Target: $$TARGET )
message( Template: $$TEMPLATE )

