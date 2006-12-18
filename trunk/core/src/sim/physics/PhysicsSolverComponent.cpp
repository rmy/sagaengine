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


#include "PhysicsSolverComponent.hpp"
#include "PhysicsComponentManager.hpp"
#include "../schema/SimSchema.hpp"
#include "../stat/SortedSimObjectList.hpp"
#include "../stat/MultiSimNodeComponent.hpp"
#include "../react/CollisionAreaComponent.hpp"
#include "../react/CollisionComponent.hpp"
#include "util/error/Log.hpp"
#include "util/bounds/BoundingBox.hpp"
#include "../thing/Actor.hpp"



namespace se_core {
	static coor_t MAX_SPEED = 64 * COOR_STEP + 2 * COOR_RES;

	PhysicsSolverComponent
	::PhysicsSolverComponent(SimComposite* owner, CollisionAreaComponent* cac) 
		: SimNodeComponent(sct_PHYSICS, owner)
		, collisionAreaComponent_(cac)
		, moverCount_(0) {
	}


	PhysicsSolverComponent
	::~PhysicsSolverComponent() {
 	}



	void PhysicsSolverComponent
	::flipChildren() {
		if(children_.isEmpty())
			return;

		MultiSimNodeComponent::TreeIterator it(children());
		while(it.hasNext()) {
			PhysicsComponent& ph = static_cast<PhysicsComponent&>(it.next());
			PosComponent* p = ph.posComponent_; //SimSchema::simObjectList.nextPosNode(itStack [ sp ]);
			Assert(p);

			// Do the flip
			ph.flip();
		}
	}


	void PhysicsSolverComponent
	::setActive(bool state) {
		if(state) {
			PhysicsComponentManager::singleton().setSolverActive(this);
		}
		else {
			moverCount_ = 0;
			PhysicsComponentManager::singleton().setSolverInactive(this);
		}
	}


	int PhysicsSolverComponent
	::performChildPhysics(PhysicsComponent** movers) {
		int moverCount = 0;
		MultiSimNodeComponent::TreeIterator it(children());
 		while(it.hasNext()) {
			PhysicsComponent& ph = static_cast<PhysicsComponent&>(it.next());

			// Calc next position
			ph.calcNextCoor();
			movers[moverCount++] = &ph;
		}
		return moverCount;
	}


	void PhysicsSolverComponent
	::affectChildren() {
		MultiSimNodeComponent::TreeIterator it(children());
		while(it.hasNext()) {
			PhysicsComponent& ph = static_cast<PhysicsComponent&>(it.next());
			ph.affect();
		}
	}


}
