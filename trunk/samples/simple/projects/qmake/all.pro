include( proj.pro )

message( "[ Application ogre_ui settings ]")

CONFIG += app
TARGET = simple

LIBS = -lgame_ogre_ui -lse_ogre -lse_client -lse_core -L$${DESTDIR} -lOgreMain -lCEGUIOgreRenderer -lCEGUIBase

DESTDIR = .

include( core.pro )
include( client.pro )
include( ogre.pro )
include( ogre_ui.pro )
include( game.pro )

message( Config: $$CONFIG )
message( Target: $$TARGET )
message( Template: $$TEMPLATE )

