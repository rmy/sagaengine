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


#include "PhysXAreaComponent.hpp"
#include "PhysXManager.hpp"
#include "PhysXThingComponent.hpp"

using namespace se_core;

namespace se_physx {

	PhysXAreaComponent
	::PhysXAreaComponent(SimComposite* owner)
		: AreaComponent(sct_ODE, owner), groundPlane_(0) {
	}


	PhysXAreaComponent
	::~PhysXAreaComponent() {
	}


	void PhysXAreaComponent
	::setActive(bool state) {
		if(state) {
			// Create a plane with default descriptor
			NxPlaneShapeDesc planeDesc;
			NxActorDesc actorDesc;

			planeDesc.normal.set(0, 1, 0);
			actorDesc.shapes.pushBack(&planeDesc);
			groundPlane_ = PhysXManager::singleton().createActor(actorDesc);

			setParent(PhysXManager::singleton());
		}
		else {
			Assert(groundPlane_);
			PhysXManager::singleton().destroyActor(*groundPlane_);
			groundPlane_ = 0;
			//PhysXManager::singleton().destroySpace(spaceId_);

			resetParent();
		}
	}


	void PhysXAreaComponent
	::update() {
		MultiSimNodeComponent::TreeIterator it(children());
		while(it.hasNext()) {
			PhysXThingComponent& otc = static_cast<PhysXThingComponent&>(it.next());
			otc.update();
		}
	}


	void PhysXAreaComponent
	::applyForces() {
		MultiSimNodeComponent::TreeIterator it(children());
		while(it.hasNext()) {
			PhysXThingComponent& otc = static_cast<PhysXThingComponent&>(it.next());
			otc.applyForces();
		}
	}


}
