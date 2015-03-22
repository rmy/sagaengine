aux_source_directory( ${SAGAENGINE}/add_on/basic/src SE_BASIC__ADD_ON_BASIC_SRC )
aux_source_directory( ${SAGAENGINE}/add_on/basic/src/basic SE_BASIC__ADD_ON_BASIC_SRC_BASIC )
aux_source_directory( ${SAGAENGINE}/add_on/basic/src/basic/schema SE_BASIC__ADD_ON_BASIC_SRC_BASIC_SCHEMA )
aux_source_directory( ${SAGAENGINE}/add_on/basic/src/basic/navmesh SE_BASIC__ADD_ON_BASIC_SRC_BASIC_NAVMESH )
aux_source_directory( ${SAGAENGINE}/add_on/basic/src/basic/action SE_BASIC__ADD_ON_BASIC_SRC_BASIC_ACTION )
aux_source_directory( ${SAGAENGINE}/add_on/basic/src/basic/thing SE_BASIC__ADD_ON_BASIC_SRC_BASIC_THING )
aux_source_directory( ${SAGAENGINE}/add_on/basic/src/basic/area SE_BASIC__ADD_ON_BASIC_SRC_BASIC_AREA )
aux_source_directory( ${SAGAENGINE}/add_on/basic/src/basic/physics SE_BASIC__ADD_ON_BASIC_SRC_BASIC_PHYSICS )
aux_source_directory( ${SAGAENGINE}/add_on/basic/src/basic/io SE_BASIC__ADD_ON_BASIC_SRC_BASIC_IO )

set (SE_BASIC ${SE_BASIC__ADD_ON_BASIC_SRC} ${SE_BASIC__ADD_ON_BASIC_SRC_BASIC} ${SE_BASIC__ADD_ON_BASIC_SRC_BASIC_SCHEMA} ${SE_BASIC__ADD_ON_BASIC_SRC_BASIC_NAVMESH} ${SE_BASIC__ADD_ON_BASIC_SRC_BASIC_ACTION} ${SE_BASIC__ADD_ON_BASIC_SRC_BASIC_THING} ${SE_BASIC__ADD_ON_BASIC_SRC_BASIC_AREA} ${SE_BASIC__ADD_ON_BASIC_SRC_BASIC_PHYSICS} ${SE_BASIC__ADD_ON_BASIC_SRC_BASIC_IO})

include_directories( ${INCLUDE_DIRECTORIES} ${SAGAENGINE}/add_on/basic/src ${SAGAENGINE}/add_on/basic/include )
