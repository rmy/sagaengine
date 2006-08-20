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
#include "util/error/StopWatch.hpp"
#include "../message/sim_message.hpp"
#include "../script/sim_script.hpp"
#include "../stat/sim_stat.hpp"
#include "../area/sim_area.hpp"
#include "../thing/sim_thing.hpp"
#include "util/system/util_system.hpp"

/**
 * The game simulation engine. This is the core of SagaEngine.
 */
namespace se_core {
	/**
	 * Global objects and methods used by the simulation logic.
	 */
	namespace SimSchema {
		_SeCoreExport bool init();
		_SeCoreExport void cleanup();


		_SeCoreExport SimListeners& engineListeners();
		_SeCoreExport InitListeners& initListeners();
		_SeCoreExport HealthListeners& healthListeners();
		_SeCoreExport SortedSimObjectList& sortedSimObjectList();

		extern _SeCoreExport ActionQueue actionQueue[];

		extern _SeCoreExport MessageCentral messageCentral;
		extern _SeCoreExport SoundCentral soundCentral;
		extern _SeCoreExport SimObjectList simObjectList;
		//extern AllPropertyLists allPropertyLists;
		extern _SeCoreExport short firstNodeSlidingActors;
		extern _SeCoreExport AreaManager areaManager;
		extern _SeCoreExport ThingManager& thingManager();
		extern _SeCoreExport Dictionary& dictionary();
		extern _SeCoreExport ShowingCutscene showingCutscene;
		extern _SeCoreExport StopWatch* stopWatch;
		extern _SeCoreExport RealClock* realClock;
		extern _SeCoreExport SimEngine simEngine;

		extern _SeCoreExport int scriptTracker;
		extern _SeCoreExport bool didTrack;
	}
}

#define SimSchema_hpp
#endif
