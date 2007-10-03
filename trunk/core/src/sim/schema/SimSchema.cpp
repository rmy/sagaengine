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
#include "comp/Composite.hpp"
#include "comp/node/RootComponent.hpp"
#include "../action/Action.hpp"
#include "../action/ActionQueue.hpp"
#include "../action/ActionManager.hpp"
#include "../script/ShowingCutscene.hpp"
#include "../script/ScriptManager.hpp"
#include "../message/MessageCentral.hpp"
#include "../message/SoundCentral.hpp"
#include "../stat/HealthListeners.hpp"
#include "../area/sim_area.hpp"
#include "../area/AreaManager.hpp"
#include "../spawn/SpawnManager.hpp"
#include "../physics/all.hpp"
#include "../react/all.hpp"
#include "../action/all.hpp"
#include "../signal/SignalManager.hpp"
#include "../stat/Dictionary.hpp"
#include "../stat/DictionaryEntry.hpp"
#include "util/system/util_system.hpp"
#include "util/error/StopWatch.hpp"
#include "util/system/RealClock.hpp"
#include <cstring>


namespace se_core {
	namespace SimSchema {
		SimEngine simEngine;
		AreaManager areaManager;

		SpawnManager& spawnManager() {
			return SpawnManager::singleton();
		}

		InitListeners& initListeners() {
			static InitListeners initListeners;
			return initListeners;
		}

		SimListeners& engineListeners() {
			static SimListeners simListeners;
			return simListeners;
		}
		

		HealthListeners& healthListeners() {
			static HealthListeners healthListeners;
			return healthListeners;
		}

		ActionQueue actionQueue[CHANNEL_COUNT];

		//StopWatch defaultStopWatch OBJECT_IN_EWRAM;
		//StopWatch* stopWatch = &defaultStopWatch;

		MessageCentral messageCentral;
		
		SoundCentral soundCentral;

		Dictionary& dictionary() {
			static Dictionary d;
			return d;
		}

		ShowingCutscene showingCutscene;
		RealClock* realClock = 0;

		const char* appName = 0;
		int scriptTracker = 0;
		bool didTrack = false;
		int speechCount = 0;


		bool init(const char* applicationName) {
			appName = applicationName;
			static DictionaryEntry dDictionaryType(DE_DICTIONARY_TYPE, DE_DICTIONARY_TYPE, "DICTIONARY_TYPE", false);
			static DictionaryEntry dTagType(DE_DICTIONARY_TYPE, DE_TAG, "TAG", false);
			static DictionaryEntry dSubstanceType(DE_DICTIONARY_TYPE, DE_SUBSTANCE, "SUBSTANCE", false);
			static DictionaryEntry dThingType(DE_DICTIONARY_TYPE, DE_THING_TYPE, "THING_TYPE", false);
			static DictionaryEntry dMovementMode(DE_DICTIONARY_TYPE, DE_MOVEMENT_MODE, "MOVEMENT_MODE", false);
			static DictionaryEntry dFirstUserType(DE_DICTIONARY_TYPE, DE_FIRST_USER_TYPE, "FIRST_USER_TYPE", false);

			PhysicsManager::singleton();
			ScriptManager::singleton();
			ActionManager::singleton();
			SpawnManager::singleton();
			CollisionManager::singleton();
			SignalManager::singleton();

			return simEngine.init();
		}

		void cleanup() {
			return simEngine.cleanup();
		}


		void touch() {
			// The core has no dependencies
		}
	}
}
