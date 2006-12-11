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
			//LogMsg(pc->pos().worldCoor() << " - " << pc->pos().localCoor());
			//LogMsg(pc->nextPos().worldCoor() << " - " << pc->nextPos().localCoor());
			//LogMsg(p << " - " << speedAndRadius);

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
		}
		else {
			resetCollisionGrid();
		}
	}


	inline bool _testCollision(CollisionComponent& pn1,
							  CollisionComponent& pn2) {
		if(!pn1.areaCovered().isTouching(pn2.areaCovered()))
			return false;

		// TODO: Investigate shape primitives

		return true;
	}




	void CollisionAreaComponent
	::getCollisionList() {
		if(!collisionGrid_)
			return;

		static const int MAX_THINGS = 256;
		static CollisionComponent* candidates[MAX_THINGS];
		Area* self = toArea();

		MultiSimNodeComponent::Iterator it(children_);
		while(it.hasNext()) {
			CollisionComponent* cc = static_cast<CollisionComponent*>(&it.next());
			if(!cc->isCollideable())
				continue;

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


				// Test collision with all collision candidates
				for(int inner = 0; inner < innerCount; ++inner) {
					if(candidates[ inner ]->toActor() == cc->toActor()) {
						continue;
					}

					CollisionComponent* pn2 = CollisionComponent::get(*candidates[ inner ]);
					if(_testCollision(*cc, *pn2)) {
						// TODO: Add contact joint
					}
				}

			}

		}



	}



}
