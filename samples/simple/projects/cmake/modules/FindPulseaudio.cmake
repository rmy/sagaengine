aux_source_directory( ${SAGAENGINE}/add_on/pulseaudio/src SE_PULSEAUDIO__ADD_ON_PULSEAUDIO_SRC )
aux_source_directory( ${SAGAENGINE}/add_on/pulseaudio/src/pulseaudio SE_PULSEAUDIO__ADD_ON_PULSEAUDIO_SRC_PULSEAUDIO )
aux_source_directory( ${SAGAENGINE}/add_on/pulseaudio/src/pulseaudio/schema SE_PULSEAUDIO__ADD_ON_PULSEAUDIO_SRC_PULSEAUDIO_SCHEMA )
aux_source_directory( ${SAGAENGINE}/add_on/pulseaudio/src/pulseaudio/lang SE_PULSEAUDIO__ADD_ON_PULSEAUDIO_SRC_PULSEAUDIO_LANG )
aux_source_directory( ${SAGAENGINE}/add_on/pulseaudio/src/pulseaudio/comp SE_PULSEAUDIO__ADD_ON_PULSEAUDIO_SRC_PULSEAUDIO_COMP )
aux_source_directory( ${SAGAENGINE}/add_on/pulseaudio/src/pulseaudio/play SE_PULSEAUDIO__ADD_ON_PULSEAUDIO_SRC_PULSEAUDIO_PLAY )
aux_source_directory( ${SAGAENGINE}/add_on/pulseaudio/src/pulseaudio/io SE_PULSEAUDIO__ADD_ON_PULSEAUDIO_SRC_PULSEAUDIO_IO )

set (SE_PULSEAUDIO ${SE_PULSEAUDIO__ADD_ON_PULSEAUDIO_SRC} ${SE_PULSEAUDIO__ADD_ON_PULSEAUDIO_SRC_PULSEAUDIO} ${SE_PULSEAUDIO__ADD_ON_PULSEAUDIO_SRC_PULSEAUDIO_SCHEMA} ${SE_PULSEAUDIO__ADD_ON_PULSEAUDIO_SRC_PULSEAUDIO_LANG} ${SE_PULSEAUDIO__ADD_ON_PULSEAUDIO_SRC_PULSEAUDIO_COMP} ${SE_PULSEAUDIO__ADD_ON_PULSEAUDIO_SRC_PULSEAUDIO_PLAY} ${SE_PULSEAUDIO__ADD_ON_PULSEAUDIO_SRC_PULSEAUDIO_IO})

include_directories( ${INCLUDE_DIRECTORIES} ${SAGAENGINE}/add_on/pulseaudio/src ${SAGAENGINE}/add_on/pulseaudio/include )
