aux_source_directory( ${SAGAENGINE}/add_on/client/src SE_CLIENT__ADD_ON_CLIENT_SRC )
aux_source_directory( ${SAGAENGINE}/add_on/client/src/client SE_CLIENT__ADD_ON_CLIENT_SRC_CLIENT )
aux_source_directory( ${SAGAENGINE}/add_on/client/src/client/event SE_CLIENT__ADD_ON_CLIENT_SRC_CLIENT_EVENT )
aux_source_directory( ${SAGAENGINE}/add_on/client/src/client/schema SE_CLIENT__ADD_ON_CLIENT_SRC_CLIENT_SCHEMA )
aux_source_directory( ${SAGAENGINE}/add_on/client/src/client/lang SE_CLIENT__ADD_ON_CLIENT_SRC_CLIENT_LANG )
aux_source_directory( ${SAGAENGINE}/add_on/client/src/client/thing SE_CLIENT__ADD_ON_CLIENT_SRC_CLIENT_THING )
aux_source_directory( ${SAGAENGINE}/add_on/client/src/client/io SE_CLIENT__ADD_ON_CLIENT_SRC_CLIENT_IO )

set (SE_CLIENT ${SE_CLIENT__ADD_ON_CLIENT_SRC} ${SE_CLIENT__ADD_ON_CLIENT_SRC_CLIENT} ${SE_CLIENT__ADD_ON_CLIENT_SRC_CLIENT_EVENT} ${SE_CLIENT__ADD_ON_CLIENT_SRC_CLIENT_SCHEMA} ${SE_CLIENT__ADD_ON_CLIENT_SRC_CLIENT_LANG} ${SE_CLIENT__ADD_ON_CLIENT_SRC_CLIENT_THING} ${SE_CLIENT__ADD_ON_CLIENT_SRC_CLIENT_IO})

include_directories( ${INCLUDE_DIRECTORIES} ${SAGAENGINE}/add_on/client/src ${SAGAENGINE}/add_on/client/include )
