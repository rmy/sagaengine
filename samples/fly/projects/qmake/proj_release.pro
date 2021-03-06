message( "[ Project settings ]" )

CONFIG = release

message( "core module included" )
INCLUDEPATH += $${SAGAENGINE}/core/include
INCLUDEPATH += $${SAGAENGINE}/core/src

message( "platform_pc add-on included" )
INCLUDEPATH += $${SAGAENGINE}/add_on/platform_pc/include
INCLUDEPATH += $${SAGAENGINE}/add_on/platform_pc/src

message( "client add-on included" )
INCLUDEPATH += $${SAGAENGINE}/add_on/client/include
INCLUDEPATH += $${SAGAENGINE}/add_on/client/src

message( "basic add-on included" )
INCLUDEPATH += $${SAGAENGINE}/add_on/basic/include
INCLUDEPATH += $${SAGAENGINE}/add_on/basic/src

message( "angelscript add-on included" )
INCLUDEPATH += $${SAGAENGINE}/add_on/angelscript/include
INCLUDEPATH += $${SAGAENGINE}/add_on/angelscript/src

message( "ogre add-on included" )
INCLUDEPATH += $${SAGAENGINE}/add_on/ogre/include
INCLUDEPATH += $${SAGAENGINE}/add_on/ogre/src

message( "fly ogre_ui included" )
INCLUDEPATH += $${SAGAENGINE}/samples/fly/ogre_ui/include
INCLUDEPATH += $${SAGAENGINE}/samples/fly/ogre_ui/src

message( "fly core included" )
INCLUDEPATH += $${SAGAENGINE}/samples/fly/logic/include
INCLUDEPATH += $${SAGAENGINE}/samples/fly/logic/src

message( "fly start included" )
INCLUDEPATH += $${SAGAENGINE}/samples/fly/start/src

message( "CEGUI included" )
INCLUDEPATH += $${CEGUI}
message( "OGRE included" )
INCLUDEPATH += $${OGRE}

DESTDIR = $${SAGAENGINE}/samples/fly/projects/qmake

DEFINES *= DEBUG_LEVEL_1
DEFINES *= SE_DOUBLE
DEFINES *= IS_OGRE
DEFINES *= _SeOgreExport= _SeBasicExport= _SeClientExport= _SePcExport= _SeCoreExport=
DEFINES *= _SimpleUiExport= _SimpleLogicExport=
DEFINES *= _FlyUiExport= _FlyLogicExport=
DEFINES *= _GameUiExport= _GameLogicExport=


message( Destdir: $$DESTDIR )
