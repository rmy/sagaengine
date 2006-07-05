include( ../../../env/qmake/proj.pro )

message( "[ ogre add-on settings ]")


CONFIG += staticlib
TARGET = se_ogre
INCLUDEPATH += ../../src
INCLUDEPATH += ../../../client/include
INCLUDEPATH += ../../../client/src
INCLUDEPATH += ../../../../core/include
INCLUDEPATH += ../../../../core/src
INCLUDEPATH += /usr/local/include/OGRE
INCLUDEPATH += /usr/local/include/CEGUI
DEFINES *= IS_OGRE



include( src.pro )

message( Config: $$CONFIG )
message( Target: $$TARGET )
message( Template: $$TEMPLATE )
