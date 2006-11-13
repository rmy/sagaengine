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
#include "util/error/Log.hpp"
#include "util/bounds/BoundingBox.hpp"
#include "../thing/Actor.hpp"



namespace se_core {
	static coor_t MAX_SPEED = 64 * COOR_STEP + 2 * COOR_RES;

	PhysicsSolverComponent
	::PhysicsSolverComponent(SimComposite* owner) 
		: SimNodeComponent(sct_PHYSICS, owner)
		, collisionGrid_(0)
		, moverCount_(0) {
	}


	PhysicsSolverComponent
	::~PhysicsSolverComponent() {
 	}


	void PhysicsSolverComponent
	::addCollideable(Thing& thing) {
		if(thing.isCollideable()) {
			if(collisionGrid_) {
				// TODO: Should use speed + radius
				coor_t speedAndRadius = thing.nextPos().radius() + MAX_SPEED;
				collisionGrid_->insert(thing.nextPos().worldCoor(), speedAndRadius, thing);
			}
		}
 	}


	void PhysicsSolverComponent
	::removeCollideable(Thing& thing) {
		if(!collisionGrid_)
			return;

		// TODO: Should use speed + radius
		coor_t speedAndRadius = thing.pos().radius() + MAX_SPEED;
		bool didDelete = collisionGrid_->remove(thing.pos().worldCoor(), speedAndRadius, thing);
		if(!didDelete) {
			coor_t speedAndRadius = thing.nextPos().radius() + MAX_SPEED;
			didDelete = collisionGrid_->remove(thing.nextPos().worldCoor(), speedAndRadius, thing);
			if(!didDelete) LogMsg("Couldn't remove " << thing.name() << " from collision grid");
		}

		// TODO: This assert fails?
		DbgAssert(didDelete);
	}


	inline bool _testActor2ThingCollision(Actor& pn1,
							  Thing& pn2) {
		BoundingBox b1(pn1.pos().worldCoor(), pn1.pos().bounds_);
		BoundingBox btmp(pn1.pos().worldCoor(), pn1.pos().bounds_);
		b1.merge(btmp);
		BoundingBox b2(pn2.pos().worldCoor(), pn2.pos().bounds_);
		BoundingBox b2tmp(pn2.pos().worldCoor(), pn2.pos().bounds_);
		b2.merge(b2tmp);

		if(!b1.isTouching(b2))
			return false;

		// Inside collision range. Collide.
		return pn1.pushThing(pn2);
	}


	void PhysicsSolverComponent
	::testActors2ThingsCollisions(PhysicsComponent** movers, short moverCount) {
		// Create buffer to temporarily hold collision candidates
		static const int MAX_THINGS = 256;
		// PS! Not thread safe, but takes less space on GBA stack
		static Thing* things[MAX_THINGS] VAR_IN_EWRAM;
		Area* self = toArea();

		for(int outer = 0; outer < moverCount; ++outer) {
			Actor* a = movers[ outer ]->actor();
			if(!a->isPusher())
				continue;

			// Get collision candidates in this and all
			// neighbouring areas
			short innerCount = 0;
			for(int n = 0; n < Area::MAX_NEIGHBOURS; ++n) {
				Area* area = self->neighbours_[ n ];
				if(!area || !area->collisionGrid()) continue;

				innerCount += area->collisionGrid()->collisionCandidates
					(a->nextPos().worldCoor(), a->nextPos().radius() + COOR_RES
					 , &things[innerCount], MAX_THINGS - innerCount);
			}

			// Test collision with all collision candidates
			for(int inner = 0; inner < innerCount; ++inner) {
				// Don't test collision with self
				if(things[ inner ] == a) {
					continue;
				}

				// Test for collision
				if(_testActor2ThingCollision(*a, *things[ inner ])) {
					a->resetFutureCoor();
					break;
				}
			}
		}
	}


	void PhysicsSolverComponent
	::setCollisionGrid(CollisionGrid* grid) {
		collisionGrid_ = grid;

		// Make sure the grid does not have any members
		// from previous usage
		collisionGrid_->clear();

		// Align the grid coordinate system with
		// this areas coordinate system
		collisionGrid_->setSize(toArea()->width(), toArea()->height());

		collisionGrid_->setOffset(toArea()->pos().worldCoor());

		// Add moving elements to grid
		SimObjectList::iterator_type it = toArea()->allThings().iterator();
		while(it != SimObjectList::end()) {
			Thing* t = SimSchema::simObjectList.nextThing(it);
			if(t->isCollideable()) {
				//LogMsg(t->name() << ": " << t->nextPos().worldCoor().toLog());
				collisionGrid_->insert(t->nextPos().worldCoor(), t->nextPos().radius(), *t);
			}
		}
	}


	void PhysicsSolverComponent
	::resetCollisionGrid() {
		CollisionGrid* g = collisionGrid_;
		if(collisionGrid_) {
			PhysicsComponentManager::singleton().releaseCollisionGrid(collisionGrid_);
			collisionGrid_ = 0;
		}
	}


	void PhysicsSolverComponent
	::flipChildren() {
		if(children_.isEmpty())
			return;

		Thing* t;


		static const int MAX_STACK_DEPTH = 10;
		MultiSimNodeComponent::Iterator itStack[ MAX_STACK_DEPTH ];
		int sp = 0;
		itStack[ 0 ].init(children_);
		do {
			// Get next in chain
			PhysicsComponent& ph = static_cast<PhysicsComponent&>(itStack[ sp ].next());
			PosNode* p = ph.owner(); //SimSchema::simObjectList.nextPosNode(itStack [ sp ]);
			Assert(p);
			// Move to new position in collision grid
			if(collisionGrid_
			   && p->isCollideable()
			   && p->pos().area() == p->nextPos().area()) {

				// TODO: Make CollisionGrid handle PosNodes
				Assert(p->isType(got_POS_NODE));
				t = static_cast<Thing*>(p);

				// TODO: Real speed instead of max speed...
				static const coor_t speed = MAX_SPEED;
				coor_t speedAndRadius = p->pos().radius() + speed;
				const Point3& wc = p->pos().worldCoor();

				// TODO: Real speed instead of max speed...
				static const coor_t nextSpeed =  MAX_SPEED;
				coor_t nextSpeedAndRadius = p->nextPos().radius() + nextSpeed;
				const Point3& nextWC = p->nextPos().worldCoor();

				collisionGrid_->move(wc, speedAndRadius, nextWC, nextSpeedAndRadius, *t);
			}

			// Do the flip
			p->flip();

			// Stack overflowing?
			Assert(sp < MAX_STACK_DEPTH - 1);

			// Push child chain as next chain on stack
			itStack[ ++sp ].init(ph.children());

			// Pop all completed chain
			while(sp >= 0 && !itStack[ sp ].hasNext()) {
				--sp;
			}
			// Continue if unpopped chains
		} while(sp >= 0);
	}


	void PhysicsSolverComponent
	::setActive(bool state) {
		if(state) {
			setCollisionGrid(PhysicsComponentManager::singleton().grabCollisionGrid());
			PhysicsComponentManager::singleton().setSolverActive(this);
		}
		else {
			moverCount_ = 0;
			resetCollisionGrid();
			PhysicsComponentManager::singleton().setSolverInactive(this);
		}
	}


	int PhysicsSolverComponent
	::performChildPhysics(PhysicsComponent** movers) {
		if(children_.isEmpty()) {
			// No children at all
			return 0;
		}

		int moverCount = 0;
		static const int MAX_STACK_DEPTH = 10;
		MultiSimNodeComponent::Iterator itStack[ MAX_STACK_DEPTH ];
		int sp = 0;
		itStack[ 0 ].init(children_);

		do {
			// Get next in chain
			PhysicsComponent& p = static_cast<PhysicsComponent&>(itStack [ sp ].next());

			// Calc next position
			if(p.calcNextCoor()) {
				// Add to movers
				movers[moverCount++] = &p;
			}


			// Push child chain as next chain on stack
			itStack[ ++sp ].init(p.children());

			// Stack overflowed?
			Assert(sp < MAX_STACK_DEPTH);

			// Pop all completed chain
			while(sp >= 0 && !itStack[ sp ].hasNext()) {
				--sp;
			}

			// Continue if there are still incomplete chains
		} while(sp >= 0);

		return moverCount;
	}


}
