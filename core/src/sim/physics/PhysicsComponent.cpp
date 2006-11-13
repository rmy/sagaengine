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


#include "PhysicsComponent.hpp"
#include "../schema/SimSchema.hpp"
#include "../stat/SortedSimObjectList.hpp"
#include "util/error/Log.hpp"
#include "../thing/Actor.hpp"



namespace se_core {
	PhysicsComponent
	::PhysicsComponent(Actor* owner, PosComponent* posComponent) 
		: SimNodeComponent(sct_PHYSICS, owner), currentPhysics_(0), posComponent_(posComponent) {
		physics_[currentPhysics_] = 0;
	}


	PhysicsComponent
	::~PhysicsComponent() {
	}


	void PhysicsComponent
	::popPhysics() {
		Assert(currentPhysics_ > 0);
		--currentPhysics_;
	}


	void PhysicsComponent
	::pushPhysics(const Physics* ph) {
		if(hasPhysics() && physics().isStacker()) {
			// Increase script counter
			++currentPhysics_;
		}
		Assert(currentPhysics_ < MAX_PHYSICS_STACK_SIZE - 1);
		physics_[ currentPhysics_ ] = ph;
	}


	void PhysicsComponent
	::pushPhysics(const char* name) {
		const Physics* p = SimSchema::sortedSimObjectList().physics(name);
		pushPhysics(p);
	}


	bool PhysicsComponent
	::calcNextCoor() {
		if(!hasPhysics()) {
			didMove_ = false;
			return false;
		}

		move_ = nextMove_;

		nextMove_.flick();
		physics().calcNext(*owner(), posComponent_->pos(), posComponent_->nextPos(), move(), nextMove());

		//
		didMove_ = posComponent_->nextPos().didParentMove() || !posComponent_->nextPos().localEquals(posComponent_->pos());
		return didMove_;
	}


	bool PhysicsComponent
	::isMover() const {
		// Actors that has physics can move
		return hasPhysics();
	}


	void PhysicsComponent
	::flip() {
		posComponent_->flip();
	}

}

