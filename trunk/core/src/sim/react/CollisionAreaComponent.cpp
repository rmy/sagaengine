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
		addChild(cc);

		if(collisionGrid_) {
			PosComponent& pos = cc.posComponent();
			// TODO: Should use speed + radius
			coor_t speedAndRadius = pos.nextPos().radius() + MAX_SPEED;
			collisionGrid_->insert(pos.nextPos().worldCoor(), speedAndRadius, cc.posComponent());
		}
 	}


	void CollisionAreaComponent
	::removeCollideable(CollisionComponent& cc) {
		removeChild(cc);

		if(!collisionGrid_)
			return;

		PosComponent& pos = cc.posComponent();

		// TODO: Should use speed + radius
		coor_t speedAndRadius = pos.pos().radius() + MAX_SPEED;
		bool didDelete = collisionGrid_->remove(pos.pos().worldCoor(), speedAndRadius, pos);
		if(!didDelete) {
			coor_t speedAndRadius = pos.nextPos().radius() + MAX_SPEED;
			didDelete = collisionGrid_->remove(pos.nextPos().worldCoor(), speedAndRadius, pos);
			AssertMsg(didDelete, "Couldn't remove " << pos.owner()->name() << " from collision grid");
		}

		if(!didDelete) {
			LogMsg(collisionGrid_->find(pos));
		}

		// TODO: This assert fails?
		DbgAssert(didDelete);
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
			CollisionComponent* c = static_cast<CollisionComponent*>(&it.next());
			PosComponent* p = static_cast<PosComponent*>(c->owner()->component(sct_POS));
			LogMsg(p->pos().worldCoor() << " - " << p->pos().localCoor());
			LogMsg(p->nextPos().worldCoor() << " - " << p->nextPos().localCoor());
			collisionGrid_->insert(p->nextPos().worldCoor(), p->nextPos().radius(), *p);
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



}
