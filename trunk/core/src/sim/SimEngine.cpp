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

#include "InitListeners.hpp"
#include "SimEngine.hpp"
#include "SimListeners.hpp"
#include "./action/ActionManager.hpp"
#include "./physics/PhysicsManager.hpp"
#include "./react/CollisionManager.hpp"
#include "./script/ScriptManager.hpp"
#include "./schema/SimSchema.hpp"
#include "./pos/Pos.hpp"
#include "./area/Area.hpp"
#include "./area/AreaManager.hpp"
#include "./signal/SignalManager.hpp"
#include "./spawn/SpawnManager.hpp"
#include "util/vecmath/Point3.hpp"
#include "util/system/RealClock.hpp"
#include "util/error/Log.hpp"
#include "util/math/Trig.hpp"
#include "util/math/CoorT.hpp"
#include "util/vecmath/Vector3.hpp"
#include "comp/schema/CompSchema.hpp"
#include "comp/Composite.hpp"
#include <cstdio>
#include <cmath>


namespace se_core {

	SimEngine
	::SimEngine()
			: previousPerform_(0)
			, isGamePaused_(false)
			, multiplePerformsPerStepEnabled_(false)
			, multiplePerformsDisabledOnce_(false)
			, lostPerformAdjustment_(0) {
		LogDetail("Creating SimEngine");
	}


	SimEngine
	::~SimEngine() {
		resetAll();
		LogDetail("Destroying SimEngine");
	}


	void SimEngine
	::setMultiplePerformsPerStepEnabled(bool state) {
		multiplePerformsPerStepEnabled_ = state;
	}


	void SimEngine
	::setLevel(const char* name) {
		nextLevel_.copy(name);
	}


	void SimEngine
	::go() {
		isGamePaused_ = false;

		//LogWarning("Capacity: " << CompSchema::voidList.capacity() << " (go 1)");
		SimSchema::initListeners().castStartGameEvent();

		while(true) {
			// Any in-game events caused the game to end?
			if(SimSchema::simEngine.isGameOver()) {
				// Will cause renderloop to end
				break;
			}

			// Translate game clock to SagaEngine format
			long when = SimSchema::realClock->millis();

			// Perform next AI steps if any is waiting
			SimSchema::simEngine.step(when);

			// Tell registered render modules to render
			SimSchema::engineListeners().castRenderEvent(SimSchema::realClock->millis());
		}
		CompSchema::activeRoot().setActive(false, true);
		level_.reset();
		SimSchema::initListeners().castCleanupLevelEvent();
		SimSchema::initListeners().castStopGameEvent();
		//LogWarning("Capacity: " << CompSchema::voidList.capacity() << " (go 2)");
	}


	bool SimEngine
	::step(long when) {
		// Check if ready to perform the next
		if(when < (previousPerform_ + 1) << TIMESTEP_INTERVAL_SHIFT) {
			return false;
		}

		// If multiple performs are disabled, adjust game clock
		// to catch up.
		long p = when >> TIMESTEP_INTERVAL_SHIFT;
		if(!multiplePerformsPerStepEnabled_ || multiplePerformsDisabledOnce_) {
			// Used to avoid jumps in the game clocks when
			// multiple steps are turned off.
			if(previousPerform_ != 0)
				lostPerformAdjustment_ += p - previousPerform_ - 1;
			previousPerform_ = p - 1;
			multiplePerformsDisabledOnce_ = false;
		}
		if(isGamePaused_ && previousPerform_) {
			lostPerformAdjustment_ += p - previousPerform_;
			previousPerform_ = p;
		}

		// w is the game clock ('when') adjusted for lost performs.
		long w = (p - lostPerformAdjustment_) << TIMESTEP_INTERVAL_SHIFT;

		// Cast PreSimTick event
		SimSchema::engineListeners().castPreSimTickEvent(w);

		// Perform actions in actionQueue until up to date...
		int performCount = 0;
		while(p > previousPerform_) {
			//! @TODO: The when may wrap after 48 days on some systems...
			if(previousPerform_ == 0) {
				// This is the first AI perform()
				previousPerform_ = p;
			}
			else {
				// Increase perform() counter
				++previousPerform_;
			}

			// Perform AI actions.
			perform(SimEngine::when());
			++performCount;
			if(performCount > 64) {
				multiplePerformsDisabledOnce_ = true;
				break;
			}
		}

		// Cast event telling listeners that the AI actions are performed,
		// and that the AI now is up to date with the game clock.
		//e.init(*this, EngineEvent::TYPE_POST_TICK, w);
		SimSchema::engineListeners().castPostSimTickEvent(w);

		return true;
	}


	void SimEngine
	::perform(long when) {
		// Performs all destructions that resulted from the actions.
		// It is necessary to do this as a separate step, because an
		// object destroyed in this initiative may have a counteraction
		// scheduled that by game rules are performed simultaneously,
		// but by the game code performed slightly later than the destroying
		// actions.
		SimSchema::spawnManager().performDestructions();

		// Updates didTrack
		ScriptManager::singleton().step(when);

		// Schedules and performs the scheduled actions in all action channels
		ActionManager::singleton().step(when);

		// Move things and check for collisions
		PhysicsManager::singleton().step(when);

		// Move things and check for collisions
		CollisionManager::singleton().step(when);

		// Move things and check for collisions
		SignalManager::singleton().step(when);

		if(!nextLevel_.equals(level_)) {
			LogWarning("New level is " << nextLevel_);
			CompSchema::activeRoot().setActive(false, true);
			if(!level_.isEmpty()) {
				SimSchema::initListeners().castCleanupLevelEvent();
			}
			level_.copy(nextLevel_.get());
			CompSchema::activeRoot().setActive(true, true);
			SimSchema::initListeners().castInitLevelEvent();

			// Don't have AI and physics catch up because
			// of skipped frames during level loading
			multiplePerformsDisabledOnce_ = true;
		}
	}


	bool SimEngine
	::init() {
		if(!SimSchema::initListeners().castPriorityInitEngineEvent())
			return false;
		if(!SimSchema::initListeners().castInitEngineEvent())
			return false;
		// Platform module must create a clock
		//Assert(SimSchema::realClock);
		return true;
	}


	void SimEngine
	::cleanup() {
		SimSchema::initListeners().castCleanupEngineEvent();
		SimSchema::initListeners().castPriorityCleanupEngineEvent();
		resetAll();
	}


	void SimEngine
	::initGame() {
		LogWarning("Capacity: " << CompSchema::voidList.capacity() << " (initGame)");
		// Reset in game engine variable that remembers the
		// 'when' of the previous step
		resetTimer();
		SimSchema::realClock->reset();
		// Begin game with game over flag not set
		setGameOver(false);
		SimSchema::initListeners().castInitGameEvent();
		CompSchema::activeRoot().init(false);
		CollisionManager::singleton().resetGodMode();
	}


	void SimEngine
	::setGameOver(bool state) {
		LogDetail(__FUNCTION__ << ": " << state);
		//static int c = 0; if(++c > 10) LogFatal("!");
		isGameOver_ = state;
	}


	void SimEngine
	::setGamePaused(bool state) {
		LogWarning(__FUNCTION__ << ": " << state << " - " << isGamePaused_);
		if(!previousPerform_ || state == isGamePaused_) {
			LogWarning("Game not started or already paused.");
			return;
		}
		isGamePaused_ = state;
		LogWarning(__FUNCTION__ << ": " << state << " - " << isGamePaused_);
		if(isGamePaused_) {
			LogWarning("Pause");
			SimSchema::initListeners().castPauseGameEvent();
		}
		else {
			LogWarning("Unpause");
			SimSchema::initListeners().castUnpauseGameEvent();
		}
	}


	void SimEngine
	::cleanupGame() {
		CompSchema::activeRoot().cleanup(false);
		for(int i = 0; i < CHANNEL_COUNT; ++i) {
			SimSchema::actionQueue[i].reset();
		}
		SimSchema::areaManager.resetThings();
		SimSchema::spawnManager().reset();
		SimSchema::initListeners().castCleanupGameEvent();
		SimSchema::areaManager.resetAll();
		SimSchema::spawnManager().resetAll();
		PhysicsManager::singleton().cleanup();


		lostPerformAdjustment_ = 0;
		previousPerform_ = 0;
		isGameOver_ = false;
		LogWarning("Capacity: " << CompSchema::voidList.capacity() << " (cleanupGame)");
	}


	void SimEngine
	::resetAll() {
		isGamePaused_ = false;
		//SimSchema::thingManager.reset();
		SimSchema::areaManager.resetAll();
		SimSchema::spawnManager().reset();
	}

}
