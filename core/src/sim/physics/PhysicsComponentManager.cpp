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


#include "PhysicsComponentManager.hpp"
#include "sim_physics.hpp"
#include "../area/Area.hpp"
#include "../area/AreaManager.hpp"
#include "../schema/SimSchema.hpp"
#include "../sim.hpp"
#include "../config/sim_config.hpp"


namespace se_core {
	PhysicsComponentManager
	::PhysicsComponentManager()
		: SimComponentManager(sct_PHYSICS) 
		, movers_(new Actor*[MAX_MOVER_COUNT])
		, moverCount_(0) {
	}

	PhysicsComponentManager
	::~PhysicsComponentManager() {
		delete[] movers_;
	}


	PhysicsComponentManager&  PhysicsComponentManager
	::singleton() {
		static PhysicsComponentManager instance;
		return instance;
	}


	void PhysicsComponentManager
	::step(long when) {
		// Precalc coordinate for the next frame
		performPhysics(when + TIMESTEP_INTERVAL);

		// Test and resolve collisions between game things and between
		// game things and walls or other terrain obstructions.
		testCollisions(when, when + TIMESTEP_INTERVAL);
	}


	void PhysicsComponentManager
	::cleanupGame() {
		//
		moverCount_ = 0;
	}


	void PhysicsComponentManager
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


	void PhysicsComponentManager
	::testCollisions(long startWhen, long endWhen) {
		for(int i = 0; i < SimSchema::areaManager.activeCount(); ++i) {
			Area* area = SimSchema::areaManager.active(i);
			area->testActors2ThingsCollisions(); //area->movers_, area->moverCount_);
		}
	}



}
