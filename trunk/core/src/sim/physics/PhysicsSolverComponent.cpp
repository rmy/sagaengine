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
		: SimComponent(sct_PHYSICS, owner)
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
		//LogMsg(thing.name());
		// TODO: This assert fails?
		DbgAssert(didDelete);
	}


	/*
	inline bool _testActor2ThingCollision(Actor& actor1,
							  Thing& thing2) {
		// How close must the things be before colliding?
		// Double for pow 2 further down
		coor_t radSum = actor1.pos().radius() + thing2.pos().radius();
		coor_double_t radSumSq = radSum;
		radSumSq *= radSumSq;

		// If not colliding when taking the movement of both things
		// into account, then no collision
		if((actor1.nextPos().worldCoor().xzDistanceSquared(thing2.nextPos().worldCoor()) > radSumSq)
				|| (actor1.nextPos().worldCoor().yDistance(thing2.nextPos().worldCoor()) > radSum)
				) {
			return false;
		}

		// Inside collision range. Definitely collide.
		return actor1.pushThing(thing2);
	}
	*/


	inline bool _testActor2ThingCollision(Actor& pn1,
							  Thing& pn2) {
		// How close must the things be before colliding?
		// Double for pow 2 further down
		/*
		coor_t r = pn1.pos().radius();
		BoundingBox b1(pn1.pos().worldCoor(), r, 2 * r);
		r = pn1.nextPos().radius();
		b1.merge(BoundingBox(pn1.nextPos().worldCoor(), r, 2 * r));

		r = pn2.pos().radius();
		BoundingBox b2(pn2.pos().worldCoor(), r, 2 * r);
		r = pn2.nextPos().radius();
		b2.merge(BoundingBox(pn2.nextPos().worldCoor(), r, 2 * r));
		*/
		BoundingBox b1(pn1.pos().worldCoor(), pn1.pos().bounds_);
		b1.merge(BoundingBox(pn1.pos().worldCoor(), pn1.pos().bounds_));
		BoundingBox b2(pn2.pos().worldCoor(), pn2.pos().bounds_);
		b2.merge(BoundingBox(pn2.pos().worldCoor(), pn2.pos().bounds_));

		//LogMsg("Collision: " << pn1.name() << ", " << pn2.name() << b1 << b2);

		if(!b1.isTouching(b2))
			return false;

		// Inside collision range. Definitely collide.
		return pn1.pushThing(pn2);
	}


	void PhysicsSolverComponent
	::testActors2ThingsCollisions(Actor** movers, short moverCount) {
		// Create buffer to temporarily hold collision candidates
		static const int MAX_THINGS = 256;
		// PS! Not thread safe, but takes less space on GBA stack
		static Thing* things[MAX_THINGS] VAR_IN_EWRAM;
		Area* self = toArea();

		for(int outer = 0; outer < moverCount; ++outer) {
			Actor* a = movers[ outer ];
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
		static const int MAX_STACK_DEPTH = 10;
		SimObjectList::iterator_type itStack[ MAX_STACK_DEPTH ];

		Thing* t;

		int sp = 0;
		itStack[ 0 ] = toArea()->childPosNodes().iterator();
		if(itStack[ 0 ] != SimObjectList::end()) {
			do {
				// Get next in chain
				PosNode* p = SimSchema::simObjectList.nextPosNode(itStack [ sp ]);
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

				// Push child chain as next chain on stack
				itStack[ ++sp ] = p->childPosNodes().iterator();

				// Stack overflowed?
				Assert(sp < MAX_STACK_DEPTH);

				// Pop all completed chain
				while(sp >= 0 && itStack[ sp ] == SimObjectList::end()) {
					--sp;
				}
				// Continue if unpopped chains
			} while(sp >= 0);
		}

		toArea()->flipChildren();
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

}
