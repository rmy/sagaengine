aux_source_directory( ../../ogre_ui/src SIMPLE_UI___OGRE_UI_SRC )
aux_source_directory( ../../ogre_ui/src/ui SIMPLE_UI___OGRE_UI_SRC_UI )
aux_source_directory( ../../ogre_ui/src/ui/schema SIMPLE_UI___OGRE_UI_SRC_UI_SCHEMA )
aux_source_directory( ../../ogre_ui/src/ui/menu SIMPLE_UI___OGRE_UI_SRC_UI_MENU )
aux_source_directory( ../../ogre_ui/src/ui/material SIMPLE_UI___OGRE_UI_SRC_UI_MATERIAL )
aux_source_directory( ../../ogre_ui/src/ui/io SIMPLE_UI___OGRE_UI_SRC_UI_IO )
aux_source_directory( ../../ogre_ui/src/ui/input SIMPLE_UI___OGRE_UI_SRC_UI_INPUT )
aux_source_directory( ../../ogre_ui/src/ui/init SIMPLE_UI___OGRE_UI_SRC_UI_INIT )

set (SIMPLE_UI ${SIMPLE_UI___OGRE_UI_SRC} ${SIMPLE_UI___OGRE_UI_SRC_UI} ${SIMPLE_UI___OGRE_UI_SRC_UI_SCHEMA} ${SIMPLE_UI___OGRE_UI_SRC_UI_MENU} ${SIMPLE_UI___OGRE_UI_SRC_UI_MATERIAL} ${SIMPLE_UI___OGRE_UI_SRC_UI_IO} ${SIMPLE_UI___OGRE_UI_SRC_UI_INPUT} ${SIMPLE_UI___OGRE_UI_SRC_UI_INIT})

include_directories( ${INCLUDE_DIRECTORIES} ../../ogre_ui/src ../../ogre_ui/include )
