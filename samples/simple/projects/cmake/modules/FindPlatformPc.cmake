aux_source_directory( ${SAGAENGINE}/add_on/platform_pc/src SE_PLATFORM__ADD_ON_PLATFORM_PC_SRC )
aux_source_directory( ${SAGAENGINE}/add_on/platform_pc/src/pc SE_PLATFORM__ADD_ON_PLATFORM_PC_SRC_PC )
aux_source_directory( ${SAGAENGINE}/add_on/platform_pc/src/pc/schema SE_PLATFORM__ADD_ON_PLATFORM_PC_SRC_PC_SCHEMA )
aux_source_directory( ${SAGAENGINE}/add_on/platform_pc/src/pc/plugin SE_PLATFORM__ADD_ON_PLATFORM_PC_SRC_PC_PLUGIN )
aux_source_directory( ${SAGAENGINE}/add_on/platform_pc/src/pc/io SE_PLATFORM__ADD_ON_PLATFORM_PC_SRC_PC_IO )

set (SE_PLATFORM ${SE_PLATFORM__ADD_ON_PLATFORM_PC_SRC} ${SE_PLATFORM__ADD_ON_PLATFORM_PC_SRC_PC} ${SE_PLATFORM__ADD_ON_PLATFORM_PC_SRC_PC_SCHEMA} ${SE_PLATFORM__ADD_ON_PLATFORM_PC_SRC_PC_PLUGIN} ${SE_PLATFORM__ADD_ON_PLATFORM_PC_SRC_PC_IO})

include_directories( ${INCLUDE_DIRECTORIES} ${SAGAENGINE}/add_on/platform_pc/src ${SAGAENGINE}/add_on/platform_pc/include )
