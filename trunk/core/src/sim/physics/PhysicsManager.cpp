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


#include "PhysicsManager.hpp"
#include "PhysicsAreaComponent.hpp"
#include "sim_physics.hpp"
#include "../area/Area.hpp"
#include "../area/AreaManager.hpp"
#include "../schema/SimSchema.hpp"
#include "../sim.hpp"
#include "../config/sim_config.hpp"


namespace se_core {
	PhysicsManager
	::PhysicsManager()
			: SimComponentManager(sct_PHYSICS) 
			, movers_(new PhysicsComponent*[MAX_MOVER_COUNT]), moverCount_(0)
			, activeSolverCount_(0) {
		activeSolvers_ = new PhysicsAreaComponent*[ MAX_ACTIVE ];
	}


	PhysicsManager
	::~PhysicsManager() {
		delete[] movers_;
	}


	PhysicsManager&  PhysicsManager
	::singleton() {
		static PhysicsManager instance;
		return instance;
	}


	void PhysicsManager
	::setSolverActive(PhysicsAreaComponent* s) {
		for(int i = 0; i < activeSolverCount_; ++i) {
			if(s == activeSolvers_[i]) return;
		}

		activeSolvers_[ activeSolverCount_ ] = s;
		++activeSolverCount_;
	}


	void PhysicsManager
	::setSolverInactive(PhysicsAreaComponent* s) {
		for(int i = 0; i < activeSolverCount_; ++i) {
			if(s == activeSolvers_[i]) {
				--activeSolverCount_;
				activeSolvers_[ i ] = activeSolvers_[ activeSolverCount_ ];
				activeSolvers_[ activeSolverCount_ ] = 0;
			}
		}
	}


	void PhysicsManager
	::step(long when) {
		// Affect thing with effects of movements
		affect(when + TIMESTEP_INTERVAL);

		// Flip coors for next physics step
		flip(when);

		// Precalc coordinate for the next frame
		performPhysics(when + TIMESTEP_INTERVAL);

		// Test and resolve collisions between game things and between
		// game things and walls or other terrain obstructions.
		//testCollisions(when, when + TIMESTEP_INTERVAL);

	}


	void PhysicsManager
	::cleanupGame() {
		//
		moverCount_ = 0;
	}


	void PhysicsManager
	::flip(long when) {
		for(int i = 0; i < activeSolverCount_; ++i) {
			PhysicsAreaComponent* s = activeSolvers_[i];
			s->flipChildren();
			s->toArea()->flipSpawns();
		}
	}


	void PhysicsManager
	::performPhysics(long when) {
		PhysicsComponent** m = movers_;
		moverCount_ = 0;
		for(int i = 0; i < activeSolverCount_; ++i) {
			PhysicsAreaComponent* s = activeSolvers_[i];
			int c = s->performChildPhysics(m);

			s->moverCount_ = c;
			s->movers_ = m;

			m += c;
			moverCount_ += c;
		}
	}




	void PhysicsManager
	::affect(long when) {
		for(int i = 0; i < activeSolverCount_; ++i) {
			PhysicsAreaComponent* s = activeSolvers_[i];
			s->affectChildren();
		}
		/*
		for(int i = 0; i < moverCount_; ++i) {
			// Affect actors with the effects of the terrain
			// they are standing on.
			// PS! Placed here to avoid an extra loop. Cannot
			// be placed in move(), because flip may move
			// the actor into a new area or no area. Placing it
			// here should have no unwanted side effects.
			PhysicsComponent* p = movers_[i];
			p->affect();
		}
		*/

	}


}

