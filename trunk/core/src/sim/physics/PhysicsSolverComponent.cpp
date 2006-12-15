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



	inline bool _testActor2ThingCollision(CollisionComponent& cc1,
							  CollisionComponent& cc2) {
		PosComponent& pn1 = cc1.posComponent();
		PosComponent& pn2 = cc2.posComponent();

		BoundingBox b1(pn1.pos().worldCoor(), pn1.pos().bounds_);
		BoundingBox btmp(pn1.nextPos().worldCoor(), pn1.nextPos().bounds_);
		b1.merge(btmp);
		BoundingBox b2(pn2.pos().worldCoor(), pn2.pos().bounds_);
		BoundingBox b2tmp(pn2.nextPos().worldCoor(), pn2.nextPos().bounds_);
		b2.merge(b2tmp);

		if(!b1.isTouching(b2))
			return false;

		if(!cc1.doesGeometryCollide(cc2))
			return false;

		// Inside collision range. Collide.
		return true;
	}


	void PhysicsSolverComponent
	::testActors2ThingsCollisions(PhysicsComponent** movers, short moverCount) {
		// Create buffer to temporarily hold collision candidates
		static const int MAX_THINGS = 256;
		// PS! Not thread safe, but takes less space on GBA stack
		static CollisionComponent* things[MAX_THINGS] VAR_IN_EWRAM;
		Area* self = toArea();

		for(int outer = 0; outer < moverCount; ++outer) {
			Actor* a = movers[ outer ]->toActor();
			if(!CollisionComponent::get(*a))
				continue;

			// Get collision candidates in this and all
			// neighbouring areas
			short innerCount = 0;
			for(int n = 0; n < Area::MAX_NEIGHBOURS; ++n) {
				Area* area = self->neighbours_[ n ];
				if(!area) continue;
				CollisionAreaComponent* cac = static_cast<CollisionAreaComponent*>(area->component(sct_COLLISION));
				if(!cac || !cac->collisionGrid()) continue;

				innerCount += cac->collisionGrid()->collisionCandidates
					(a->nextPos().worldCoor(), a->nextPos().radius() + COOR_RES
					 , &things[innerCount], MAX_THINGS - innerCount);
			}

			// Test collision with all collision candidates
			for(int inner = 0; inner < innerCount; ++inner) {
				// Don't test collision with self
				if(things[ inner ]->toActor() == a) {
					continue;
				}

				// Test for collision
				CollisionComponent& cc1 = *CollisionComponent::get(*movers_[ outer ]);
				CollisionComponent& cc2 = *things[ inner ];

				// Only collide once (and at least once)
				if(&cc1 < &cc2 && cc1.isCollideable() && cc2.isCollideable()) {
					continue;
				}

				// Does collide
				if(_testActor2ThingCollision(cc1, cc2)) {
					// Event
					cc1.pushThing(cc2);
					cc2.pushThing(cc1);
				}
			}
		}
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
