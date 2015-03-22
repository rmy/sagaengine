aux_source_directory( ${SAGAENGINE}/add_on/audiere/src SE_AUDIERE__ADD_ON_AUDIERE_SRC )
aux_source_directory( ${SAGAENGINE}/add_on/audiere/src/audiere SE_AUDIERE__ADD_ON_AUDIERE_SRC_AUDIERE )
aux_source_directory( ${SAGAENGINE}/add_on/audiere/src/audiere/schema SE_AUDIERE__ADD_ON_AUDIERE_SRC_AUDIERE_SCHEMA )
aux_source_directory( ${SAGAENGINE}/add_on/audiere/src/audiere/lang SE_AUDIERE__ADD_ON_AUDIERE_SRC_AUDIERE_LANG )
aux_source_directory( ${SAGAENGINE}/add_on/audiere/src/audiere/comp SE_AUDIERE__ADD_ON_AUDIERE_SRC_AUDIERE_COMP )
aux_source_directory( ${SAGAENGINE}/add_on/audiere/src/audiere/play SE_AUDIERE__ADD_ON_AUDIERE_SRC_AUDIERE_PLAY )
aux_source_directory( ${SAGAENGINE}/add_on/audiere/src/audiere/io SE_AUDIERE__ADD_ON_AUDIERE_SRC_AUDIERE_IO )

set (SE_AUDIERE ${SE_AUDIERE__ADD_ON_AUDIERE_SRC} ${SE_AUDIERE__ADD_ON_AUDIERE_SRC_AUDIERE} ${SE_AUDIERE__ADD_ON_AUDIERE_SRC_AUDIERE_SCHEMA} ${SE_AUDIERE__ADD_ON_AUDIERE_SRC_AUDIERE_LANG} ${SE_AUDIERE__ADD_ON_AUDIERE_SRC_AUDIERE_COMP} ${SE_AUDIERE__ADD_ON_AUDIERE_SRC_AUDIERE_PLAY} ${SE_AUDIERE__ADD_ON_AUDIERE_SRC_AUDIERE_IO})

include_directories( ${INCLUDE_DIRECTORIES} ${SAGAENGINE}/add_on/audiere/src ${SAGAENGINE}/add_on/audiere/include )
