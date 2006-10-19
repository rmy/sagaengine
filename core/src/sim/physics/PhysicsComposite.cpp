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


#include "PhysicsComposite.hpp"
#include "../schema/SimSchema.hpp"
#include "../stat/SortedSimObjectList.hpp"
#include "util/error/Log.hpp"



namespace se_core {
	PhysicsComposite
	::PhysicsComposite(Actor* owner) 
		: SimComposite(sct_PHYSICS, owner), currentPhysics_(0) {
		physics_[currentPhysics_] = 0;
	}


	PhysicsComposite
	::~PhysicsComposite() {
	}


	void PhysicsComposite
	::popPhysics() {
		Assert(currentPhysics_ > 0);
		--currentPhysics_;
	}


	void PhysicsComposite
	::pushPhysics(const Physics* ph) {
		if(hasPhysics() && physics().isStacker()) {
			// Increase script counter
			++currentPhysics_;
		}
		Assert(currentPhysics_ < MAX_PHYSICS_STACK_SIZE - 1);
		physics_[ currentPhysics_ ] = ph;
	}


	void PhysicsComposite
	::pushPhysics(const char* name) {
		const Physics* p = SimSchema::sortedSimObjectList().physics(name);
		pushPhysics(p);
	}

}

