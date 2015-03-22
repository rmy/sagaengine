aux_source_directory( ../../logic/src SIMPLE_LOGIC___LOGIC_SRC )
aux_source_directory( ../../logic/src/logic SIMPLE_LOGIC___LOGIC_SRC_LOGIC )
aux_source_directory( ../../logic/src/logic/schema SIMPLE_LOGIC___LOGIC_SRC_LOGIC_SCHEMA )
aux_source_directory( ../../logic/src/logic/action SIMPLE_LOGIC___LOGIC_SRC_LOGIC_ACTION )
aux_source_directory( ../../logic/src/logic/physics SIMPLE_LOGIC___LOGIC_SRC_LOGIC_PHYSICS )

set (SIMPLE_LOGIC ${SIMPLE_LOGIC___LOGIC_SRC} ${SIMPLE_LOGIC___LOGIC_SRC_LOGIC} ${SIMPLE_LOGIC___LOGIC_SRC_LOGIC_SCHEMA} ${SIMPLE_LOGIC___LOGIC_SRC_LOGIC_SCRIPT} ${SIMPLE_LOGIC___LOGIC_SRC_LOGIC_STAT} ${SIMPLE_LOGIC___LOGIC_SRC_LOGIC_ACTION} ${SIMPLE_LOGIC___LOGIC_SRC_LOGIC_SIGNAL} ${SIMPLE_LOGIC___LOGIC_SRC_LOGIC_CONFIG} ${SIMPLE_LOGIC___LOGIC_SRC_LOGIC_PHYSICS} ${SIMPLE_LOGIC___LOGIC_SRC_LOGIC_COLLIDE} ${SIMPLE_LOGIC___LOGIC_SRC_LOGIC_IO})

include_directories( ${INCLUDE_DIRECTORIES} ../../logic/src ../../logic/include )
