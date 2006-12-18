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

#include "CollisionAreaComponent.hpp"
#include "CollisionManager.hpp"
#include "CollisionComponent.hpp"
#include "../schema/SimSchema.hpp"
#include "../stat/SortedSimObjectList.hpp"
#include "util/error/Log.hpp"
#include "util/bounds/BoundingBox.hpp"
#include "../thing/Actor.hpp"
#include "../pos/PosComponent.hpp"



namespace se_core {
	static coor_t MAX_SPEED = 64 * COOR_STEP + 2 * COOR_RES;

	CollisionAreaComponent
	::CollisionAreaComponent(SimComposite* owner) 
		: SimNodeComponent(sct_COLLISION, owner)
		, collisionGrid_(0) {
	}


	CollisionAreaComponent
	::~CollisionAreaComponent() {
 	}


	void CollisionAreaComponent
	::addCollideable(CollisionComponent& cc) {
		if(collisionGrid_) {
			Point3 p;
			cc.areaCovered().center(p);
			coor_t speedAndRadius = cc.areaCovered().radius();

			PosComponent* pc = &cc.posComponent();
			collisionGrid_->insert(p, speedAndRadius, cc);
		}
 	}


	void CollisionAreaComponent
	::removeCollideable(CollisionComponent& cc) {
		if(!collisionGrid_)
			return;

		Point3 p;
		cc.areaCovered().center(p);
		coor_t speedAndRadius = cc.areaCovered().radius();
		bool didDelete = collisionGrid_->remove(p, speedAndRadius, cc);

		AssertFatal(didDelete, "Couldn't remove " << cc.owner()->name() << " from collision grid (" << collisionGrid_->find(cc) << ")");
	}


	void CollisionAreaComponent
	::setCollisionGrid(CollisionGrid* grid) {
		collisionGrid_ = grid;

		// Make sure the grid does not have any members
		// from previous usage
		collisionGrid_->clear();

		// Align the grid coordinate system with
		// this areas coordinate system
		collisionGrid_->setSize(toArea()->width(), toArea()->height());

		collisionGrid_->setOffset(toArea()->pos().worldCoor());

		// Add collideable elements to grid
		MultiSimNodeComponent::Iterator it(children_);
		while(it.hasNext()) {
			CollisionComponent* cc = static_cast<CollisionComponent*>(&it.next());
			if(!cc->isCollideable())
				continue;

			cc->updateAreaCovered();

			Point3 newPos;
			cc->areaCovered().center(newPos);
			coor_t newRadius = cc->areaCovered().radius();


			PosComponent* p = &cc->posComponent();
			//LogMsg(p->pos().worldCoor() << " - " << p->pos().localCoor());
			//LogMsg(p->nextPos().worldCoor() << " - " << p->nextPos().localCoor());
			//LogMsg(newPos << " - " << newRadius);
			collisionGrid_->insert(newPos, newRadius, *cc);
		}
	}


	void CollisionAreaComponent
	::resetCollisionGrid() {
		CollisionGrid* g = collisionGrid_;
		if(collisionGrid_) {
			CollisionManager::singleton().releaseCollisionGrid(collisionGrid_);
			collisionGrid_ = 0;
		}
	}


	void CollisionAreaComponent
	::setActive(bool state) {
		if(state) {
			setCollisionGrid(CollisionManager::singleton().grabCollisionGrid());
			setParent(CollisionManager::singleton());
		}
		else {
			resetParent();
			resetCollisionGrid();
		}
	}


	inline bool _testCollision(CollisionComponent& cc1,
							  CollisionComponent& cc2) {
		if(!cc1.areaCovered().isTouching(cc2.areaCovered()))
			return false;


		if(!cc1.doesGeometryCollide(cc2))
			return false;

		// Inside collision range. Collide.
		return true;
	}




	int CollisionAreaComponent
	::getContactList(Contact* list, int maxCollisions) {
		int count = 0;

		if(!collisionGrid_)
			return count;

		static const int MAX_THINGS = 256;
		static CollisionComponent* candidates[MAX_THINGS];
		Area* self = toArea();

		MultiSimNodeComponent::Iterator it(children_);
		while(it.hasNext()) {
			CollisionComponent* cc = static_cast<CollisionComponent*>(&it.next());

			short innerCount = 0;
			for(int n = 0; n < Area::MAX_NEIGHBOURS; ++n) {
				Area* area = self->neighbours_[ n ];
				if(!area) continue;
				CollisionAreaComponent* cac = static_cast<CollisionAreaComponent*>(area->component(sct_COLLISION));
				if(!cac || !cac->collisionGrid()) continue;

				Point3 p;
				cc->areaCovered().center(p);
				coor_t speedAndRadius = cc->areaCovered().radius();
				
				innerCount += cac->collisionGrid()->collisionCandidates
					(p, speedAndRadius, &candidates[innerCount], MAX_THINGS - innerCount);
			}


			// Test collision with all collision candidates
			for(int inner = 0; inner < innerCount; ++inner) {
				// Don't test collision with self
				if(candidates[ inner ] == cc) {
					continue;
				}

				// Test for collision
				CollisionComponent& cc2 = *candidates[ inner ];

				// Only collide once (and at least once)
				if(cc < &cc2 && cc->isCollideable() && cc2.isCollideable()) {
					continue;
				}

				if(_testCollision(*cc, cc2)) {
					Assert(count < maxCollisions);
					list[ count ].cc1_ = cc;
					list[ count ].cc2_ = &cc2;
					++count;
				}
			}
		}

		return count;
	}


}
