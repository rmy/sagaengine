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
#include "PhysicsSolverComponent.hpp"
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
			, movers_(new Actor*[MAX_MOVER_COUNT]), moverCount_(0)
			, activeSolverCount_(0)
			, gridCount_(0), gridPoolCount_(0) {
		collisionGrids_ = new CollisionGrid*[ MAX_ACTIVE ];
		gridPool_ = new CollisionGrid*[ MAX_ACTIVE ];
		activeSolvers_ = new PhysicsSolverComponent*[ MAX_ACTIVE ];
	}


	PhysicsComponentManager
	::~PhysicsComponentManager() {
		for(int i = 0; i < gridCount_; ++i) {
			delete collisionGrids_[i];
		}
		gridCount_ = 0;
		LogMsg("Destroyed area grids");

		delete[] movers_;
	}


	PhysicsComponentManager&  PhysicsComponentManager
	::singleton() {
		static PhysicsComponentManager instance;
		return instance;
	}


	void PhysicsComponentManager
	::setSolverActive(PhysicsSolverComponent* s) {
		for(int i = 0; i < activeSolverCount_; ++i) {
			if(s == activeSolvers_[i]) return;
		}

		activeSolvers_[ activeSolverCount_ ] = s;
		++activeSolverCount_;
	}


	void PhysicsComponentManager
	::setSolverInactive(PhysicsSolverComponent* s) {
		for(int i = 0; i < activeSolverCount_; ++i) {
			if(s == activeSolvers_[i]) {
				--activeSolverCount_;
				activeSolvers_[ i ] = activeSolvers_[ activeSolverCount_ ];
				activeSolvers_[ activeSolverCount_ ] = 0;
			}
		}
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
		/*
		for(int i = 0; i < SimSchema::areaManager.activeCount(); ++i) {
			Area* area = SimSchema::areaManager.active(i);
			area->flipChildren();
		}
		*/
		for(int i = 0; i < activeSolverCount_; ++i) {
			PhysicsSolverComponent* s = activeSolvers_[i];
			s->flipChildren();
		}
	}


	void PhysicsComponentManager
	::performPhysics(long when) {
		Actor** m = movers_;
		moverCount_ = 0;
		/*
		for(int i = 0; i < SimSchema::areaManager.activeCount(); ++i) {
			Area* area = SimSchema::areaManager.active(i);
			int c = performChildPhysics(area, m);

			area->moverCount_ = c;
			area->movers_ = m;

			m += c;
			moverCount_ += c;

		}
		*/

		for(int i = 0; i < activeSolverCount_; ++i) {
			PhysicsSolverComponent* s = activeSolvers_[i];
			int c = performChildPhysics(s->toArea(), m);

			s->moverCount_ = c;
			s->movers_ = m;

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
		/*
		for(int i = 0; i < SimSchema::areaManager.activeCount(); ++i) {
			Area* area = SimSchema::areaManager.active(i);
			area->testActors2ThingsCollisions(area->movers_, area->moverCount_);
		}
		*/
		for(int i = 0; i < activeSolverCount_; ++i) {
			PhysicsSolverComponent* s = activeSolvers_[i];
			s->testActors2ThingsCollisions(s->movers_, s->moverCount_);
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



	CollisionGrid* PhysicsComponentManager
	::grabCollisionGrid() {
		// Create grid object if necessary
		coor_tile_t maxWidth = 1, maxHeight = 1;
		if(!gridPoolCount_) {
			Assert(gridCount_ < MAX_ACTIVE);

			coor_tile_t maxWidth = SimSchema::areaManager.maxWidth();
			coor_tile_t maxHeight = SimSchema::areaManager.maxHeight();
			short d = 2;
			while((1 << (d + 1)) < maxWidth / 4 && (1 << (d + 1)) < maxHeight / 4)
				++d;

			CollisionGrid* g = new CollisionGrid(maxWidth, maxHeight, d);
			gridPool_[ gridPoolCount_++ ] = g;
			collisionGrids_[ gridCount_++ ] = g;
		}

		return gridPool_[ --gridPoolCount_ ];
	}



	void PhysicsComponentManager
	::releaseCollisionGrid(CollisionGrid* g) {
		g->clear();
		gridPool_[ gridPoolCount_++ ] = g;
	}


}
