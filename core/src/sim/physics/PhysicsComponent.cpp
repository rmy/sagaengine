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
#include "../react/CollisionComponent.hpp"
#include "../schema/SimSchema.hpp"
#include "../stat/SortedSimObjectList.hpp"
#include "util/error/Log.hpp"
#include "../thing/Actor.hpp"



namespace se_core {
	PhysicsComponent
	::PhysicsComponent(Actor* owner, PosComponent* posComponent) 
		: SimNodeComponent(sct_PHYSICS, owner), currentPhysics_(0), affect_(0), posComponent_(posComponent) {
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
	::clearPhysics() {
		currentPhysics_ = 0;
		physics_[0] = 0;
	}

	void PhysicsComponent
	::pushPhysics(const Physics* ph) {
		if(hasDefaultPhysics() && physics().isStacker()) {
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
		if(!hasDefaultPhysics()) {
			didMove_ = false;
			affect_ = 0;
			return false;
		}

		move_ = nextMove_;
		physics().calcNext(*toActor(), posComponent_->pos(), posComponent_->nextPos(), move(), nextMove());
		move_ = nextMove_;
		nextMove_.resetForces();

		//
		didMove_ = posComponent_->nextPos().didParentMove() || !posComponent_->nextPos().localEquals(posComponent_->pos());

		// Move to new position in collision grid
		CollisionComponent* cc = CollisionComponent::get(*this);
		if(cc && cc->isCollideable() && posComponent_->pos().area() == posComponent_->nextPos().area()) {
			cc->move();
		}
		affect_ = &physics();

		return didMove_;
	}


	bool PhysicsComponent
	::isMover() const {
		// Actors that has physics can move
		return hasDefaultPhysics();
	}


	void PhysicsComponent
	::flip() {
		posComponent_->flip();
	}

}

