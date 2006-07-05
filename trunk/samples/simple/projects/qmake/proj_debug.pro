message( "[ Project settings ]")

SAGAENGINE = /home/rune/svn/sf
OGRE = /usr/local/include/OGRE
CEGUI = /usr/local/include/CEGUI
ANGELSCRIPT = /usr/local/src/angelscript/sdk/angelscript/include/

CONFIG = debug

message( "core module included" )
INCLUDEPATH += $${SAGAENGINE}/core/include
INCLUDEPATH += $${SAGAENGINE}/core/src

message( "client add-on included" )
INCLUDEPATH += $${SAGAENGINE}/add_on/client/include
INCLUDEPATH += $${SAGAENGINE}/add_on/client/src

message( "angelscript add-on included" )
INCLUDEPATH += $${SAGAENGINE}/add_on/angelscript/include
INCLUDEPATH += $${SAGAENGINE}/add_on/angelscript/src

message( "ogre add-on included" )
INCLUDEPATH += $${SAGAENGINE}/add_on/ogre/include
INCLUDEPATH += $${SAGAENGINE}/add_on/ogre/src

message( "game ogre_ui included" )
INCLUDEPATH += $${SAGAENGINE}/samples/simple/ogre_ui/include
INCLUDEPATH += $${SAGAENGINE}/samples/simple/ogre_ui/src

message( "game core included" )
INCLUDEPATH += $${SAGAENGINE}/samples/simple/game/include
INCLUDEPATH += $${SAGAENGINE}/samples/simple/game/src

message( "CEGUI included" )
INCLUDEPATH += $${CEGUI}
message( "OGRE included" )
INCLUDEPATH += $${OGRE}

DESTDIR = $${SAGAENGINE}/samples/simple/lib

DEFINES *= IS_OGRE

message(Destdir: $$DESTDIR )
