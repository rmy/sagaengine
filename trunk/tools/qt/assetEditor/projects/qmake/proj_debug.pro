message( "[ Project settings ]")

CONFIG *= debug

message( "core module included" )
INCLUDEPATH += $${SAGAENGINE}/core/include
INCLUDEPATH += $${SAGAENGINE}/core/src

message( "platform_oc add-on included" )
INCLUDEPATH += $${SAGAENGINE}/add_on/platform_pc/include
INCLUDEPATH += $${SAGAENGINE}/add_on/platform_pc/src

message( "start included" )
INCLUDEPATH += $${SAGAENGINE}/tools/qt/assetEditor/src

message( "CEGUI included" )
INCLUDEPATH += $${CEGUI}
message( "OGRE included" )
INCLUDEPATH += $${OGRE}

DESTDIR = $${SAGAENGINE}/tools/qt/assetEditor/projects/qmake

DEFINES *= IS_OGRE DEBUG_LEVEL_2 SE_STATIC
DEFINES *= _SeOgreExport= _SeBasicExport= _SeClientExport= _SeCoreExport= _SePcExport=
DEFINES *= _SimpleUiExport= _SimpleLogicExport=

message(Destdir: $$DESTDIR )
