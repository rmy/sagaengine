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
		// Flip coors for next physics step
		flip(when);

		// Precalc coordinate for the next frame
		performPhysics(when + TIMESTEP_INTERVAL);

		// Test and resolve collisions between game things and between
		// game things and walls or other terrain obstructions.
		testCollisions(when, when + TIMESTEP_INTERVAL);

		// Affect thing with effects of movements
		affect(when + TIMESTEP_INTERVAL);
	}


	void PhysicsComponentManager
	::cleanupGame() {
		//
		moverCount_ = 0;
	}

	void PhysicsComponentManager
	::flip(long when) {
		for(int i = 0; i < SimSchema::areaManager.activeCount(); ++i) {
			Area* area = SimSchema::areaManager.active(i);
			area->flipChildren();
		}
	}


	void PhysicsComponentManager
	::performPhysics(long when) {
		Actor** m = movers_;
		moverCount_ = 0;
		for(int i = 0; i < SimSchema::areaManager.activeCount(); ++i) {
			Area* area = SimSchema::areaManager.active(i);
			int c = performChildPhysics(area, m);

			area->moverCount_ = c;
			area->movers_ = m;

			m += c;
			moverCount_ += c;

		}
	}


	int PhysicsComponentManager
	::performChildPhysics(Area* area, Actor** movers) {
		int moverCount = 0;

		static const int MAX_STACK_DEPTH = 10;
		SimObjectList::iterator_type itStack[ MAX_STACK_DEPTH ];

		int sp = 0;
		itStack[ 0 ] = area->childPosNodes().iterator();
		if(itStack[ 0 ] == SimObjectList::end())
			// No children at all
			return 0;

		do {
			// Get next in chain
			Actor* a = SimSchema::simObjectList.nextActor(itStack [ sp ]);

			// Calc next position
			if(a->calcNextCoor()) {
				// Add to movers
				movers[moverCount++] = a;
			}


			// Push child chain as next chain on stack
			itStack[ ++sp ] = a->childPosNodes().iterator();

			// Stack overflowed?
			Assert(sp < MAX_STACK_DEPTH);

			// Pop all completed chain
			while(sp >= 0 && itStack[ sp ] == SimObjectList::end()) {
				--sp;
			}

			// Continue if there are still incomplete chains
		} while(sp >= 0);

		return moverCount;
	}


	void PhysicsComponentManager
	::testCollisions(long startWhen, long endWhen) {
		for(int i = 0; i < SimSchema::areaManager.activeCount(); ++i) {
			Area* area = SimSchema::areaManager.active(i);
			area->testActors2ThingsCollisions(area->movers_, area->moverCount_);
		}
	}


	void PhysicsComponentManager
	::affect(long when) {
		for(int i = 0; i < moverCount_; ++i) {
			// Affect actors with the effects of the terrain
			// they are standing on.
			// PS! Placed here to avoid an extra loop. Cannot
			// be placed in move(), because flip may move
			// the actor into a new area or no area. Placing it
			// here should have no unwanted side effects.
			Actor* a = movers_[i];
			a->affect();
		}

	}


}
