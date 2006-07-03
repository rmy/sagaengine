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


#ifndef SimSchema_hpp

#include "../sim.hpp"
#include "../action/sim_action.hpp"
#include "../error/sim_error.hpp"
#include "../message/sim_message.hpp"
#include "../script/sim_script.hpp"
#include "../stat/sim_stat.hpp"
#include "../area/sim_area.hpp"
#include "../thing/sim_thing.hpp"
#include "util/system/util_system.hpp"

namespace se_core {
	namespace SimSchema {
		bool init();
		void cleanup();


		SimListeners& engineListeners();
		InitListeners& initListeners();
		HealthListeners& healthListeners();
		SortedSimObjectList& sortedSimObjectList();

		extern ActionQueue actionQueue[];

		extern MessageCentral messageCentral;
		extern SimObjectList simObjectList;
		//extern AllPropertyLists allPropertyLists;
		extern short firstNodeSlidingActors;
		extern AreaManager areaManager;
		extern ThingManager& thingManager();
		extern Dictionary& dictionary();
		extern ShowingCutscene showingCutscene;
		extern StopWatch* stopWatch;
		extern RealClock* realClock;
		extern SimEngine simEngine;

		extern int scriptTracker;
		extern bool didTrack;
	}
}

#define SimSchema_hpp
#endif
