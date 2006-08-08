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
#include "./action/Action.hpp"
#include "./action/ActionQueue.hpp"
#include "./error/StopWatch.hpp"
#include "./schema/SimSchema.hpp"
#include "./stat/Pos.hpp"
#include "./stat/Force.hpp"
#include "./stat/MultiSimObject.hpp"
#include "./stat/MultiSimObjectIterator.hpp"
#include "./stat/SimObjectList.hpp"
#include "./area/Area.hpp"
#include "./area/AreaManager.hpp"
#include "./thing/Thing.hpp"
#include "./thing/ThingManager.hpp"
#include "./thing/Actor.hpp"
#include "./thing/Camera.hpp"
#include "util/vecmath/Point3.hpp"
#include "util/system/RealClock.hpp"
#include "util/error/Log.hpp"
#include "util/math/Trig.hpp"
#include "util/math/CoorT.hpp"
#include <cstdio>
#include <cmath>


namespace se_core {

	SimEngine
	::SimEngine()
		: previousPerform_(0), multiplePerformsPerStepEnabled_(false)
		, lostPerformAdjustment_(0)
		, movers_(new Actor*[MAX_MOVER_COUNT])
		, moverCount_(0)
		, specialMover_(0) {
		LogMsg("Creating SimEngine");
	}


	SimEngine
	::~SimEngine() {
		delete[] movers_;
		resetAll();
		LogMsg("Destroying SimEngine");
	}


	SimEngine* SimEngine
	::singleton() {
		return &SimSchema::simEngine;
	}


	inline void SimEngine
	::flip(long when) {
		for(int i = 0; i < SimSchema::areaManager.activeCount(); ++i) {
			Area* area = SimSchema::areaManager.active(i);
			area->flipChildren();
		}
	}


	void SimEngine
	::setMultiplePerformsPerStepEnabled(bool state) {
		multiplePerformsPerStepEnabled_ = state;
	}


	bool SimEngine
	::step(long when) {
		// Check if ready to perform the next
		if(when < (previousPerform_ + 1) * TIMESTEP_INTERVAL) {
			return false;
		}

		// If multiple performs are disabled, adjust game clock
		// to catch up.
		long p = when >> TIMESTEP_INTERVAL_SHIFT;
		if(!multiplePerformsPerStepEnabled_) {
			// Used to avoid jumps in the game clocks when
			// multiple steps are turned off.
			if(previousPerform_ != 0)
				lostPerformAdjustment_ += p - previousPerform_ - 1;
			previousPerform_ = p - 1;
		}

		// w is the game clock ('when') adjusted for lost performs.
		long w = (p - lostPerformAdjustment_) << TIMESTEP_INTERVAL_SHIFT;

		// Cast PreSimTickt event
		//e.init(*this, EngineEvent::TYPE_PRE_TICK, w);
		SimSchema::engineListeners().castPreSimTickEvent(w);

		// Perform actions in actionQueue until up to date...
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
		}

		// Cast event telling listeners that the AI actions are performed,
		// and that the AI now is up to date with the game clock.
		//e.init(*this, EngineEvent::TYPE_POST_TICK, w);
		SimSchema::engineListeners().castPostSimTickEvent(w);

		return true;
	}


	void SimEngine
	::perform(long when) {
		// Make the precalced next coor the present position.
		flip(when);

		// Performs the scheduled actions in all action channels
		for(int i = 0; i < CHANNEL_COUNT; ++i) {
			SimSchema::actionQueue[i].performScheduledActions(when);
		}

		// Precalc coordinate for the next frame
		performPhysics(when + TIMESTEP_INTERVAL);

		// Test and resolve collisions between game things and between
		// game things and walls or other terrain obstructions.
		testCollisions(when, when + TIMESTEP_INTERVAL);

		// Performs all destructions that resulted form the actions.
		// It is necessary to do this as a separate step, because an
		// object destroyed in this initiative may have a counteraction
		// scheduled that by game rules are performed simultaneously,
		// but by the game code performed slightly later than the destroying
		// actions.
		SimSchema::thingManager().performDestructions();

		// Schedule next action after after all actors have performed theirs,
		// as to not disadvantage the AI for any actor. (Will call scripts
		// to ask what the next action should be.)
		for(int i = 0; i < CHANNEL_COUNT; ++i) {
			SimSchema::actionQueue[i].scheduleNextActions(when);
		}

		// Step to the next step in any performed cutscene, if one
		// is active.
		if(SimSchema::didTrack) {
			++SimSchema::scriptTracker;
			SimSchema::didTrack = false;
		}

		// Prepares the next initiative (that is, the next scheduled actions)
		// in all action channels
		for(int i = 0; i < CHANNEL_COUNT; ++i) {
			SimSchema::actionQueue[i].beginNextInitiative();
		}

	}


	void SimEngine
	::performPhysics(long when) {
		Actor** m = movers_;
		moverCount_ = 0;
		for(int i = 0; i < SimSchema::areaManager.activeCount(); ++i) {
			Area* area = SimSchema::areaManager.active(i);
			int c = area->performChildPhysics(m);
			m += c;
			moverCount_ += c;
		}
	}


	void SimEngine
	::testCollisions(long startWhen, long endWhen) {
		for(int i = 0; i < SimSchema::areaManager.activeCount(); ++i) {
			Area* area = SimSchema::areaManager.active(i);
			area->testActors2ThingsCollisions(); //area->movers_, area->moverCount_);
		}
	}

	bool SimEngine
	::init() {
		SimSchema::initListeners().castInitEngineEvent();
		// Platform module must create a clock
		Assert(SimSchema::realClock);
		return true;
	}

	void SimEngine
	::cleanup() {
		SimSchema::cleanup();
		SimSchema::initListeners().castCleanupEngineEvent();
		resetAll();
	}

	void SimEngine
	::initGame() {
		// Reset in game engine variable that remembers the
		// 'when' of the previous step
		resetTimer();
		SimSchema::realClock->reset();
		// Begin game with game over flag not set
		setGameOver(false);
		SimSchema::didTrack = true;
		SimSchema::scriptTracker = 0;

		SimSchema::initListeners().castInitGameEvent();

	}


	void SimEngine
	::setGameOver(bool state) {
		LogMsg(__FUNCTION__ << ": " << state);
		isGameOver_ = state;
	}


	void SimEngine
	::cleanupGame() {
		SimSchema::initListeners().castCleanupGameEvent();

		setSpecialMover(0);
		perform(SimEngine::when() + TIMESTEP_INTERVAL);
		SimSchema::areaManager.resetThings();
		for(int i = 0; i < CHANNEL_COUNT; ++i) {
			SimSchema::actionQueue[i].reset();
		}
		SimSchema::thingManager().reset();
		SimSchema::didTrack = false;
		SimSchema::scriptTracker = 0;
		lostPerformAdjustment_ = 0;
		previousPerform_ = 0;
		isGameOver_ = false;
		moverCount_ = 0;
	}


	void SimEngine
	::resetAll() {
		//SimSchema::thingManager.reset();
		SimSchema::areaManager.resetAll();
		SimSchema::thingManager().reset();
	}
}
