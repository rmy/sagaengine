!include( ../../../../env/qmake/env.pro ) {
    include( ../../../../env/qmake/missing.pro )
}

message( "[ Application ogre_ui settings ]")

CONFIG += app
TARGET = test

#LIBS = -L$${DESTDIR}

include( core.pro )
include( test.pro )


INCLUDEPATH =
include( proj.pro )
DESTDIR = .
OBJECTS_DIR = .obj

message( Config: $$CONFIG )
message( DestDir: $$DESTDIR )
message( Target: $$TARGET )
message( Template: $$TEMPLATE )
