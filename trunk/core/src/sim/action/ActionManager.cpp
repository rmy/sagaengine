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


#include "ActionManager.hpp"
#include "Action.hpp"
#include "ActionQueue.hpp"
#include "sim_action.hpp"
#include "../schema/SimSchema.hpp"



namespace se_core {

	void ActionManager
	::step(long when) {
		// Schedule next action after after all actors have performed theirs,
		// as to not disadvantage the AI for any actor. (Will call scripts
		// to ask what the next action should be.)
		for(int i = 0; i < CHANNEL_COUNT; ++i) {
			SimSchema::actionQueue[i].scheduleNextActions(when);
		}

		// Prepares the next initiative (that is, the next scheduled actions)
		// in all action channels
		for(int i = 0; i < CHANNEL_COUNT; ++i) {
			SimSchema::actionQueue[i].beginNextInitiative();
		}

		// Performs the scheduled actions in all action channels
		for(int i = 0; i < CHANNEL_COUNT; ++i) {
			SimSchema::actionQueue[i].performScheduledActions(when);
		}

	}


	void ActionManager
	::cleanupGame() {
		//
		for(int i = 0; i < CHANNEL_COUNT; ++i) {
			SimSchema::actionQueue[i].reset();
		}
	}


	ActionManager&  ActionManager
	::singleton() {
		static ActionManager instance;
		return instance;
	}

}
