aux_source_directory( ${SAGAENGINE}/add_on/editor/src SE_EDITOR__ADD_ON_EDITOR_SRC )
aux_source_directory( ${SAGAENGINE}/add_on/editor/src/editor SE_EDITOR__ADD_ON_EDITOR_SRC_EDITOR )
aux_source_directory( ${SAGAENGINE}/add_on/editor/src/editor/schema SE_EDITOR__ADD_ON_EDITOR_SRC_EDITOR_SCHEMA )
aux_source_directory( ${SAGAENGINE}/add_on/editor/src/editor/comp SE_EDITOR__ADD_ON_EDITOR_SRC_EDITOR_COMP )
aux_source_directory( ${SAGAENGINE}/add_on/editor/src/editor/action SE_EDITOR__ADD_ON_EDITOR_SRC_EDITOR_ACTION )
aux_source_directory( ${SAGAENGINE}/add_on/editor/src/editor/physics SE_EDITOR__ADD_ON_EDITOR_SRC_EDITOR_PHYSICS )
aux_source_directory( ${SAGAENGINE}/add_on/editor/src/editor/io SE_EDITOR__ADD_ON_EDITOR_SRC_EDITOR_IO )
aux_source_directory( ${SAGAENGINE}/add_on/editor/src/editor/input SE_EDITOR__ADD_ON_EDITOR_SRC_EDITOR_INPUT )

set (SE_EDITOR ${SE_EDITOR__ADD_ON_EDITOR_SRC} ${SE_EDITOR__ADD_ON_EDITOR_SRC_EDITOR} ${SE_EDITOR__ADD_ON_EDITOR_SRC_EDITOR_SCHEMA} ${SE_EDITOR__ADD_ON_EDITOR_SRC_EDITOR_COMP} ${SE_EDITOR__ADD_ON_EDITOR_SRC_EDITOR_ACTION} ${SE_EDITOR__ADD_ON_EDITOR_SRC_EDITOR_PHYSICS} ${SE_EDITOR__ADD_ON_EDITOR_SRC_EDITOR_IO} ${SE_EDITOR__ADD_ON_EDITOR_SRC_EDITOR_INPUT})

include_directories( ${INCLUDE_DIRECTORIES} ${SAGAENGINE}/add_on/editor/src ${SAGAENGINE}/add_on/editor/include )
