aux_source_directory( ${SAGAENGINE}/core/src SE_CORE__CORE_SRC )
aux_source_directory( ${SAGAENGINE}/core/src/util SE_CORE__CORE_SRC_UTIL )
aux_source_directory( ${SAGAENGINE}/core/src/util/bounds SE_CORE__CORE_SRC_UTIL_BOUNDS )
aux_source_directory( ${SAGAENGINE}/core/src/util/system SE_CORE__CORE_SRC_UTIL_SYSTEM )
aux_source_directory( ${SAGAENGINE}/core/src/util/template SE_CORE__CORE_SRC_UTIL_TEMPLATE )
aux_source_directory( ${SAGAENGINE}/core/src/util/noise SE_CORE__CORE_SRC_UTIL_NOISE )
aux_source_directory( ${SAGAENGINE}/core/src/util/type SE_CORE__CORE_SRC_UTIL_TYPE )
aux_source_directory( ${SAGAENGINE}/core/src/util/error SE_CORE__CORE_SRC_UTIL_ERROR )
aux_source_directory( ${SAGAENGINE}/core/src/util/config SE_CORE__CORE_SRC_UTIL_CONFIG )
aux_source_directory( ${SAGAENGINE}/core/src/util/math SE_CORE__CORE_SRC_UTIL_MATH )
aux_source_directory( ${SAGAENGINE}/core/src/util/vecmath SE_CORE__CORE_SRC_UTIL_VECMATH )
aux_source_directory( ${SAGAENGINE}/core/src/util/task SE_CORE__CORE_SRC_UTIL_TASK )
aux_source_directory( ${SAGAENGINE}/core/src/comp SE_CORE__CORE_SRC_COMP )
aux_source_directory( ${SAGAENGINE}/core/src/comp/schema SE_CORE__CORE_SRC_COMP_SCHEMA )
aux_source_directory( ${SAGAENGINE}/core/src/comp/node SE_CORE__CORE_SRC_COMP_NODE )
aux_source_directory( ${SAGAENGINE}/core/src/comp/list SE_CORE__CORE_SRC_COMP_LIST )
aux_source_directory( ${SAGAENGINE}/core/src/io SE_CORE__CORE_SRC_IO )
aux_source_directory( ${SAGAENGINE}/core/src/io/schema SE_CORE__CORE_SRC_IO_SCHEMA )
aux_source_directory( ${SAGAENGINE}/core/src/io/parse SE_CORE__CORE_SRC_IO_PARSE )
aux_source_directory( ${SAGAENGINE}/core/src/io/parse/component SE_CORE__CORE_SRC_IO_PARSE_COMPONENT )
aux_source_directory( ${SAGAENGINE}/core/src/io/stream SE_CORE__CORE_SRC_IO_STREAM )
aux_source_directory( ${SAGAENGINE}/core/src/io/encode SE_CORE__CORE_SRC_IO_ENCODE )
aux_source_directory( ${SAGAENGINE}/core/src/sim SE_CORE__CORE_SRC_SIM )
aux_source_directory( ${SAGAENGINE}/core/src/sim/schema SE_CORE__CORE_SRC_SIM_SCHEMA )
aux_source_directory( ${SAGAENGINE}/core/src/sim/zone SE_CORE__CORE_SRC_SIM_ZONE )
aux_source_directory( ${SAGAENGINE}/core/src/sim/script SE_CORE__CORE_SRC_SIM_SCRIPT )
aux_source_directory( ${SAGAENGINE}/core/src/sim/stat SE_CORE__CORE_SRC_SIM_STAT )
aux_source_directory( ${SAGAENGINE}/core/src/sim/message SE_CORE__CORE_SRC_SIM_MESSAGE )
aux_source_directory( ${SAGAENGINE}/core/src/sim/action SE_CORE__CORE_SRC_SIM_ACTION )
aux_source_directory( ${SAGAENGINE}/core/src/sim/custom SE_CORE__CORE_SRC_SIM_CUSTOM )
aux_source_directory( ${SAGAENGINE}/core/src/sim/thing SE_CORE__CORE_SRC_SIM_THING )
aux_source_directory( ${SAGAENGINE}/core/src/sim/signal SE_CORE__CORE_SRC_SIM_SIGNAL )
aux_source_directory( ${SAGAENGINE}/core/src/sim/area SE_CORE__CORE_SRC_SIM_AREA )
aux_source_directory( ${SAGAENGINE}/core/src/sim/config SE_CORE__CORE_SRC_SIM_CONFIG )
aux_source_directory( ${SAGAENGINE}/core/src/sim/spawn SE_CORE__CORE_SRC_SIM_SPAWN )
aux_source_directory( ${SAGAENGINE}/core/src/sim/physics SE_CORE__CORE_SRC_SIM_PHYSICS )
aux_source_directory( ${SAGAENGINE}/core/src/sim/setting SE_CORE__CORE_SRC_SIM_SETTING )
aux_source_directory( ${SAGAENGINE}/core/src/sim/cutscene SE_CORE__CORE_SRC_SIM_CUTSCENE )
aux_source_directory( ${SAGAENGINE}/core/src/sim/react SE_CORE__CORE_SRC_SIM_REACT )
aux_source_directory( ${SAGAENGINE}/core/src/sim/pos SE_CORE__CORE_SRC_SIM_POS )

set (SE_CORE ${SE_CORE__CORE_SRC} ${SE_CORE__CORE_SRC_UTIL} ${SE_CORE__CORE_SRC_UTIL_BOUNDS} ${SE_CORE__CORE_SRC_UTIL_SYSTEM} ${SE_CORE__CORE_SRC_UTIL_TEMPLATE} ${SE_CORE__CORE_SRC_UTIL_NOISE} ${SE_CORE__CORE_SRC_UTIL_TYPE} ${SE_CORE__CORE_SRC_UTIL_ERROR} ${SE_CORE__CORE_SRC_UTIL_CONFIG} ${SE_CORE__CORE_SRC_UTIL_MATH} ${SE_CORE__CORE_SRC_UTIL_VECMATH} ${SE_CORE__CORE_SRC_UTIL_TASK} ${SE_CORE__CORE_SRC_COMP} ${SE_CORE__CORE_SRC_COMP_SCHEMA} ${SE_CORE__CORE_SRC_COMP_NODE} ${SE_CORE__CORE_SRC_COMP_LIST} ${SE_CORE__CORE_SRC_IO} ${SE_CORE__CORE_SRC_IO_SCHEMA} ${SE_CORE__CORE_SRC_IO_PARSE} ${SE_CORE__CORE_SRC_IO_PARSE_COMPONENT} ${SE_CORE__CORE_SRC_IO_STREAM} ${SE_CORE__CORE_SRC_IO_ENCODE} ${SE_CORE__CORE_SRC_SIM} ${SE_CORE__CORE_SRC_SIM_SCHEMA} ${SE_CORE__CORE_SRC_SIM_ZONE} ${SE_CORE__CORE_SRC_SIM_SCRIPT} ${SE_CORE__CORE_SRC_SIM_STAT} ${SE_CORE__CORE_SRC_SIM_MESSAGE} ${SE_CORE__CORE_SRC_SIM_ACTION} ${SE_CORE__CORE_SRC_SIM_CUSTOM} ${SE_CORE__CORE_SRC_SIM_THING} ${SE_CORE__CORE_SRC_SIM_SIGNAL} ${SE_CORE__CORE_SRC_SIM_AREA} ${SE_CORE__CORE_SRC_SIM_CONFIG} ${SE_CORE__CORE_SRC_SIM_SPAWN} ${SE_CORE__CORE_SRC_SIM_PHYSICS} ${SE_CORE__CORE_SRC_SIM_SETTING} ${SE_CORE__CORE_SRC_SIM_CUTSCENE} ${SE_CORE__CORE_SRC_SIM_REACT} ${SE_CORE__CORE_SRC_SIM_POS})

include_directories( ${INCLUDE_DIRECTORIES} ${SAGAENGINE}/core/src ${SAGAENGINE}/core/include )
