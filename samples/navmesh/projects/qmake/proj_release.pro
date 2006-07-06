message( "[ Project settings ]")

SAGAENGINE = /home/rune/svn/sf
OGRE = /usr/local/include/OGRE
CEGUI = /usr/local/include/CEGUI
ANGELSCRIPT = /usr/local/src/angelscript/sdk/angelscript/include/

CONFIG = release

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
INCLUDEPATH += ../../ogre_ui/include
INCLUDEPATH += ../../ogre_ui/src

message( "game core included" )
INCLUDEPATH += ../../game/include
INCLUDEPATH += ../../game/src

message( "CEGUI included" )
INCLUDEPATH += $${CEGUI}
message( "OGRE included" )
INCLUDEPATH += $${OGRE}
message( "Angelscript included" )
INCLUDEPATH += $${ANGELSCRIPT}

DESTDIR = $${SAGAENGINE}/samples/simple/lib

DEFINES *= IS_OGRE

message(Destdir: $$DESTDIR )
