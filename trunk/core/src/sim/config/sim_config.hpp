/*
SagaEngine library
Copyright (c) 2002-2006 Skalden Studio AS

This software is provided 'as-is', without any express or implied 
warranty. In no event will the authors be held liable for any 
damages arising from the use of this software.

Permission is granted to distribute the library under the terms of the 
Q Public License version 1.0. Be sure to read and understand the license
before using the library. It should be included here, or you may read it
at http://www.trolltech.com/products/qt/licenses/licensing/qpl

The original version of this library can be located at:
http://www.sagaengine.com/

Rune Myrland
rune@skalden.com
*/


#ifndef engine_config_engine_config_hpp
#define engine_config_engine_config_hpp

#include "util/config/util_config.hpp"

namespace se_core {

// You may configure this with compiler option
#ifndef SE_MAX_GT
#	define SE_MAX_GT 512
#endif

// You may configure this with a compiler option
#ifndef SE_MAX_GO
#	define SE_MAX_GO 4096
#endif


// Or by custom config file	(see util/config/util_config.hpp)
// for how it works.
#ifndef SE_CONFIG_CUSTOM_GO_COUNT
	const int MAX_GAME_TYPES = SE_MAX_GT;
	const int MAX_GAME_OBJECTS = SE_MAX_GO;
#endif

// You may configure this with a compiler option. The interval between
// simulation engine timestep will be 2 ^ SE_TIMESTEP_SHIFT milliseconds.
#ifndef SE_STEP_SHIFT
//#	define SE_STEP_SHIFT 6 // 16 steps a second
#	define SE_STEP_SHIFT 5 // 32 steps a second
//#	define SE_STEP_SHIFT 4 // 64 steps a second
#endif


// Or by custom config file	(see util/config/util_config.hpp)
// for how it works.
#ifndef SE_CONFIG_CUSTOM_TIMESTEP_INTERVAL
	const short TIMESTEP_INTERVAL_SHIFT = SE_STEP_SHIFT;
#endif
	const long TIMESTEP_INTERVAL = 1L << TIMESTEP_INTERVAL_SHIFT;

	
	//const scale_t SCALE_WHEN_TO_SECOND = (SCALE_RES / 1024);
}


namespace se_core {
	enum DictonaryType {
		DE_DICTIONARY_TYPE,
		DE_THING_TYPE,
		DE_SINGLE_VALUE,
		DE_ATTRIBUTE,
		DE_MULTI_SIM_OBJECT,
		DE_MOVEMENT_MODE,
		DE_TERRAIN_STYLE,
		DE_SOUND_TYPE,
		DE_PHRASE_TYPE,
		DE_FIGHTING_STYLE,
		DE_FIRST_USER_TYPE
	};

}


/* Game logic enums doesn't belong in se_core
#ifndef SE_CONFIG_CUSTOM_THING
namespace se_core {

	enum Thing_SingleValueType {
		SV_UNDEFINED = 0,
		SV_ENTRANCE_ID,
		SV_TAG,
		SV_PUSHER_TYPE,
		SV_PUSHED_TYPE,
		SV_SPAWN_COUNT,
		SV_DIRECTION,
		SV_ROTATE,
		SV_SOUND_SCHEMA,
		SV_MUSIC, // se_gba
		SV_DYNAMIC_FLOOR, // se_gba
		SV_COUNT
	};

	enum Thing_AttributeType {
		ATT_UNDEFINED,
		ATT_ENTER_TO,
		ATT_SCRIPT,
		ATT_COUNT
	};

	enum Thing_MultiSimObjectTypeOld {
		MGO_THINGS_CARRIED = 0,
		MGO_THINGS_WORN,
		MGO_CUTSCENES,
		MGO_CUTSCENE_MEMBERSHIPS,
		MGO_QUEST_GOALS,
		MGO_COUNT
	};

	enum Thing_MultiSimObjectType {
		MGOT_THINGS_CARRIED = 0,
		MGOT_THINGS_WORN,
		MGOT_CUTSCENES,
		MGOT_CUTSCENE_MEMBERSHIPS,
		MGOT_QUEST_GOALS,
		MGOT_COUNT
	};

}
#endif
*/


#endif
