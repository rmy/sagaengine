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
			return simEngine.init();
		}

		void cleanup() {
			return simEngine.cleanup();
		}
	}
}
