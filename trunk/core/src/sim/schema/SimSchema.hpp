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
		/**
		 * Same as SimSchema::simEngine.init()
		 * @param applicationName application name which add-on modules may use
		 *                        to choose application specific resources.
		 */
		_SeCoreExport bool init(const char* applicationName);

		/**
		 * Same as SimSchema::simEngine.cleanup()
		 */
		_SeCoreExport void cleanup();

		/**
		 * The main simulation engine object.
		 * 
		 * SimSchema::simEngine.init() - init simulation engine and registered add-on modules
		 * SimSchema::simEngine.initGame() - init simulation engine before new game
		 * SimSchema::simEngine.step() - step the simulation engine one timestep forward
		 * SimSchema::simEngine.go() - enter a gameloop that lasts until the game is over
		 * SimSchema::simEngine.setGameOver() - sets the game as over
		 * SimSchema::simEngine.cleanupGame() - cleanup simulation engine after a game
		 * SimSchema::simEngine.cleanup() - cleanup simulation engine and registered add-on modules
		 */
		extern _SeCoreExport SimEngine simEngine;

		/**
		 * Manager for areas.
		 *
		 * You should register your AreaFactory in the area manager. And it is also
		 * this class you ask if you want to create a new area, or wonder wether it
		 * exists.
		 */
		extern _SeCoreExport AreaManager areaManager;

		/**
		 * Manager for Thing, Actor, Camera and Player objects.
		 *
		 * Responsible for creating and destroying things. You should
		 * register your ThingFactory objects here.
		 */		
		extern _SeCoreExport ThingManager& thingManager();

		/**
		 * Manager for init listeners.
		 * Init events are called when the application requests the engine to be
		 * initialized or cleaned up, or when a new game is initalized or cleaned up.
		 */
		_SeCoreExport InitListeners& initListeners();

		/**
		 * Manager of engine events.
		 * If you need to be notified before and after simulation steps, register
		 * your listener here.
		 */
		_SeCoreExport SimListeners& engineListeners();

		/**
		 * Manager for health listeners.
		 * Health listeners gives casts events whenever the health of the player changes.
		 */
		_SeCoreExport HealthListeners& healthListeners();

		/**
		 * Manager for speech and message events.
		 * Events are cast whenever an actor has requested it wants to speak through
		 * the Actor::speak(...) method, or  a MessageCentral::inform(...) has been called.
		 * Typical listeners would be Ui speech bubbles or consoles, or perhaps a sound
		 * player that plays sounds for speech.
		 * 
		 * @see se_client::Phrase
		 */
		extern _SeCoreExport MessageCentral messageCentral;

		/**
		 * Manager for sound events.
		 * A sound event is cast whenever an actor has requested to make a sound.
		 */
		extern _SeCoreExport SoundCentral soundCentral;
		
		/**
		 * List of named instances of SimObject. 
		 * If you need a script or physics object by name instead of by object reference, this
		 * is where you can query for them. This is an important resource for file parsers
		 * and factory classes.
		 */
		_SeCoreExport SortedSimObjectList& sortedSimObjectList();

		/**
		 * Dictionary of string to int conversions.
		 *
		 * enums does often have use for a string representation of their values,
		 * for use in input scripts. The dictionary is where you register and query
		 * for such values.
		 * 
		 * Values are grouped.
		 */
		extern _SeCoreExport Dictionary& dictionary();

		/**
		 * Object containing the real time.
		 * This pointer must be initialized by a platform dependent add-on module.
		 */
		extern _SeCoreExport RealClock* realClock;
		
		/**
		 * Manager for actions that are to be performed.
		 * Whenever an actor wants to perform an actor, it registers itself in an ActionQueue.
		 * There are CHANNEL_COUNT action queues, enabling an actor to have this
		 * many actions in progress at once. You will not access this object yourself. Instead
		 * you will call the Actor::planAction method.
		 */
		extern _SeCoreExport ActionQueue actionQueue[];

		/**
		 * Container for singly linked list containing instances of SimObject.
		 * This list is used a lot by the MultiSimObject class.
		 */
		extern _SeCoreExport SimObjectList simObjectList;
		extern _SeCoreExport SimComponentList simComponentList;

		//extern AllPropertyLists allPropertyLists;
		//extern _SeCoreExport StopWatch* stopWatch;

		extern _SeCoreExport ShowingCutscene showingCutscene;
		extern _SeCoreExport int scriptTracker;
		extern _SeCoreExport bool didTrack;

		/** Force linking of dependencies */
		void _SeCoreExport touch();

		/**
		 * Application name, as registered by the application during
		 * init. Add-ons may use this to identify application specific
		 * resources. The platform_pc add-on uses it to choose which datapath
		 * file belongs to the application.
		 */
		extern _SeCoreExport const char* appName;
	}
}

#define SimSchema_hpp
#endif
