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


#include "ScriptComponentManager.hpp"
#include "sim_script.hpp"
#include "../schema/SimSchema.hpp"
#include "../sim.hpp"
#include "../config/sim_config.hpp"


namespace se_core {
	ScriptComponentManager
	::ScriptComponentManager()
		: SimComponentManager(sct_SCRIPT) {
	}

	ScriptComponentManager
	::~ScriptComponentManager() {
	}


	ScriptComponentManager&  ScriptComponentManager
	::singleton() {
		static ScriptComponentManager instance;
		return instance;
	}


	void ScriptComponentManager
	::step(long when) {
		// Step to the next step in any performed cutscene, if one
		// is active.
		if(SimSchema::didTrack) {
			++SimSchema::scriptTracker;
			SimSchema::didTrack = false;
		}
	}


	void ScriptComponentManager
	::initGame() {
		// Reset in game engine variable that remembers the
		// 'when' of the previous step
		SimSchema::didTrack = true;
		SimSchema::scriptTracker = 0;
	}


	void ScriptComponentManager
	::cleanupGame() {
		SimSchema::didTrack = false;
		SimSchema::scriptTracker = 0;
	}

}
