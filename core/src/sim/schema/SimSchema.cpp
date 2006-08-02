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


#include "SimSchema.hpp"
#include "../InitListeners.hpp"
#include "../SimListeners.hpp"
#include "../SimEngine.hpp"
#include "../action/Action.hpp"
#include "../action/ActionQueue.hpp"
#include "../script/ShowingCutscene.hpp"
#include "../error/StopWatch.hpp"
#include "../message/MessageCentral.hpp"
#include "../message/SoundCentral.hpp"
#include "../stat/HealthListeners.hpp"
#include "../stat/SimObjectList.hpp"
#include "../stat/SortedSimObjectList.hpp"
#include "../area/sim_area.hpp"
#include "../area/AreaManager.hpp"
#include "../thing/ThingManager.hpp"
#include "util/system/util_system.hpp"
#include "util/system/RealClock.hpp"
#include "../physics/all.hpp"
#include "../action/all.hpp"
#include "../stat/Dictionary.hpp"
#include "../stat/DictionaryEntry.hpp"
#include <cstring>


namespace se_core {
	namespace SimSchema {
		SortedSimObjectList& sortedSimObjectList() {
			static SortedSimObjectList sgol;
			return sgol;
		}

		SimListeners& engineListeners() {
			static SimListeners simListeners;
			return simListeners;
		}

		InitListeners& initListeners() {
			static InitListeners initListeners;
			return initListeners;
		}

		HealthListeners& healthListeners() {
			static HealthListeners healthListeners;
			return healthListeners;
		}

		ActionQueue actionQueue[CHANNEL_COUNT];

		StopWatch defaultStopWatch OBJECT_IN_EWRAM;
		StopWatch* stopWatch = &defaultStopWatch;

		MessageCentral messageCentral;
		SoundCentral soundCentral;

		SimObjectList simObjectList;
		Dictionary& dictionary() {
			static Dictionary d;
			return d;
		}
		//extern AllPropertyLists allPropertyLists;

		AreaManager areaManager OBJECT_IN_EWRAM;
		ThingManager& thingManager() {
			static ThingManager thingManager;
			return thingManager;
		}

		ShowingCutscene showingCutscene;
		RealClock* realClock = 0;
		SimEngine simEngine;

		int scriptTracker = 0;
		bool didTrack = false;


		bool init() {
			static const PhDefault physicsDefault;

			/* TODO: Move these out.
#ifndef SE_CONFIG_CUSTOM_MOVEMENT
			static const DictionaryEntry mmBlocked(DE_MOVEMENT_MODE, BLOCKED, "BLOCKED");
			static const DictionaryEntry mmStand(DE_MOVEMENT_MODE, STAND, "STAND");
			static const DictionaryEntry mmWalk(DE_MOVEMENT_MODE, WALK, "WALK");
#endif

			static const DictionaryEntry svUndefined(DE_SINGLE_VALUE, SV_UNDEFINED, "UNDEFINED");
			static const DictionaryEntry svEntranceId(DE_SINGLE_VALUE, SV_ENTRANCE_ID, "ENTRANCE_ID");
			static const DictionaryEntry svTag(DE_SINGLE_VALUE, SV_TAG, "TAG");
			static const DictionaryEntry svPusherType(DE_SINGLE_VALUE, SV_PUSHER_TYPE, "PUSHER_TYPE");
			static const DictionaryEntry svPushedType(DE_SINGLE_VALUE, SV_PUSHED_TYPE, "PUSHED_TYPE");
			static const DictionaryEntry svSpawnCount(DE_SINGLE_VALUE, SV_SPAWN_COUNT, "SPAWN_COUNT");
			static const DictionaryEntry svDirection(DE_SINGLE_VALUE, SV_DIRECTION, "DIRECTION");
			static const DictionaryEntry svRotate(DE_SINGLE_VALUE, SV_ROTATE, "ROTATE");
			static const DictionaryEntry svSoundSchema(DE_SINGLE_VALUE, SV_SOUND_SCHEMA, "SOUND_SCHEMA");
			static const DictionaryEntry svMusic(DE_SINGLE_VALUE, SV_MUSIC, "MUSIC");
			static const DictionaryEntry svDynamicFloor(DE_SINGLE_VALUE, SV_DYNAMIC_FLOOR, "DYNAMIC_FLOOR");

			static const DictionaryEntry attUndefined(DE_ATTRIBUTE, ATT_UNDEFINED, "UNDEFINED");
			static const DictionaryEntry attEnterTo(DE_ATTRIBUTE, ATT_ENTER_TO, "ENTER_TO");
			static const DictionaryEntry attScript(DE_ATTRIBUTE, ATT_SCRIPT, "SCRIPT");


			static const DictionaryEntry fsDefault(DE_FIGHTING_STYLE, 0, "DEFAULT");
			static const DictionaryEntry fsOffGuard(DE_FIGHTING_STYLE, 0, "OFF_GUARD");
			static const DictionaryEntry fsShortRange(DE_FIGHTING_STYLE, 1, "SHORT_RANGE");
			static const DictionaryEntry fsMediumRange(DE_FIGHTING_STYLE, 2, "MEDIUM_RANGE");
			static const DictionaryEntry fsLongRange(DE_FIGHTING_STYLE, 3, "LONG_RANGE");
			*/

			return true;
		}

		void cleanup() {
		}
	}
}
