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


#include "CollisionComponent.hpp"
#include "CollisionAreaComponent.hpp"
#include "../schema/SimSchema.hpp"
#include "../stat/SortedSimObjectList.hpp"
#include "util/error/Log.hpp"
#include "../thing/Actor.hpp"
#include "../react/all.hpp"


namespace se_core {
	CollisionComponent
	::CollisionComponent(Actor* owner, PosComponent* posComponent)
		: AreaChildComponent(sct_COLLISION, owner)
		, posComponent_(posComponent)
		, isCollideable_(false) {
	}


	CollisionComponent
	::CollisionComponent(Actor* owner)
		: AreaChildComponent(sct_COLLISION, owner)
		, isCollideable_(false) {
		posComponent_ = static_cast<PosComponent*>(owner_->component(sct_POS));
		Assert(posComponent_);
	}


	CollisionComponent
	::~CollisionComponent() {
	}


	void  CollisionComponent
	::setCollideable(bool isCollideable) {
		if(isCollideable_ == isCollideable)
			return;

		isCollideable_ = isCollideable;
		if(parent_) {
			CollisionAreaComponent* cac = static_cast<CollisionAreaComponent*>(parent_);
			if(isCollideable_) {
				updateAreaCovered();
				cac->addCollideable(*this);
			}
			else {
				cac->removeCollideable(*this);
			}
		}
	}


	void CollisionComponent
	::areaChanged(SimComposite* newArea, SimComposite* oldArea) {
		if(oldArea) {
			resetParent();
			if(isCollideable_) {
				CollisionAreaComponent* cac = static_cast<CollisionAreaComponent*>(oldArea->component(type()));
				cac->removeCollideable(*this);
			}
		}
		if(newArea) {
			CollisionAreaComponent* cac = static_cast<CollisionAreaComponent*>(newArea->component(type()));
			setParent(*cac);
			if(isCollideable_) {
				updateAreaCovered();
				cac->addCollideable(*this);
			}
		}
	}


	void CollisionComponent
	::updateAreaCovered() {
 		BoundingBox toBox(posComponent_->nextPos().worldCoor(), posComponent_->nextPos().bounds_);
		areaCovered_ = toBox;
		if(posComponent_->pos().area() == posComponent_->nextPos().area()) {
			BoundingBox fromBox(posComponent_->pos().worldCoor(), posComponent_->pos().bounds_);
			areaCovered_.merge(fromBox);
		}
	}


	void CollisionComponent
	::move() {
		Point3 oldPos, newPos;
		coor_t oldRadius = areaCovered().radius();
		areaCovered().center(oldPos);

		updateAreaCovered();

		coor_t newRadius = areaCovered().radius();
		areaCovered().center(newPos);

		CollisionAreaComponent* const cac = static_cast<CollisionAreaComponent*>(parent_);
		Assert(cac->collisionGrid());
		cac->collisionGrid()->move(oldPos, oldRadius, newPos, newRadius, *this);
	}

}
