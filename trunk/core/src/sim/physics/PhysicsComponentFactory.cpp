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


#include "PhysicsComponentFactory.hpp"
#include "PhysicsComponent.hpp"
#include "../sim.hpp"
#include "../SimEngine.hpp"
#include "../config/all.hpp"
#include "../stat/all.hpp"


namespace se_core {

	PhysicsComponentFactory
	::PhysicsComponentFactory()
		: ComponentFactory(sct_PHYSICS) {
	}


	Component* PhysicsComponentFactory
	::create(Composite* owner) const {
		// Try to get existing component 
		// - allows overrides of default values
		// (Useful when loading saved games).
		PhysicsComponent::Ptr c(*owner);
		if(c == 0) {
			// Create new component
			c = new PhysicsComponent(owner, this);
		}

		c->nextMove() = move_;
		c->setDefaultPhysics(physics_);

		return c;
	}

	void PhysicsComponentFactory
	::setPhysics(const char* name) {
		physics_ = Physics::lookup(name);
	}


	void PhysicsComponentFactory
	::setMass(scale_t m) {
		move_.mass_ = m * SCALE_RES;
	}


	void PhysicsComponentFactory
	::setFriction(float linear, float angular, float bounceDecay) {
		move_.linearFriction_ = linear * SCALE_STEP;
		move_.angularFriction_ = angular * SCALE_STEP;
		move_.bounceMaintain_ = 1 - bounceDecay;
	}


}
