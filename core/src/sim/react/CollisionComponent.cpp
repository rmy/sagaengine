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
#include "util/bounds/BoundingBox.hpp"
#include "util/bounds/BoundingCylinder.hpp"


namespace se_core {
	CollisionComponent
	::CollisionComponent(Actor* owner, PosComponent* posComponent)
		: AreaChildComponent(sct_COLLISION, owner)
		, posComponent_(posComponent)
		, isCollideable_(false)
		, ignore_(0)
		, geometryType_(geom_CYLINDER) {
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
		if(!parent_)
			return;

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


	bool CollisionComponent
	::doesGeometryCollide(const CollisionComponent& other) const {
		// Only cylinder support at the moment
		const PosComponent& pusher = posComponent();
		const PosComponent& target = other.posComponent();

		if(geometryType() == geom_CYLINDER && other.geometryType() == geom_CYLINDER) {
			BoundingCylinder b1(pusher.nextPos().worldCoor(), pusher.nextPos().bounds_);
			BoundingCylinder b2(pusher.nextPos().worldCoor(), pusher.nextPos().bounds_);
			return b2.isTouching(b1);
		}

		if(geometryType() == geom_BOX && other.geometryType() == geom_CYLINDER) {
			BoundingBox b1(pusher.nextPos().worldCoor(), pusher.nextPos().bounds_);
			BoundingCylinder b2(pusher.nextPos().worldCoor(), pusher.nextPos().bounds_);
			return b2.isTouching(b1);
		}

		if(geometryType() == geom_CYLINDER && other.geometryType() == geom_BOX) {
			BoundingCylinder b1(pusher.nextPos().worldCoor(), pusher.nextPos().bounds_);
			BoundingBox b2(target.nextPos().worldCoor(), target.nextPos().bounds_);
			return b1.isTouching(b2);
		}

		if(geometryType() == geom_BOX && other.geometryType() == geom_BOX) {
			BoundingBox b1(pusher.nextPos().worldCoor(), pusher.nextPos().bounds_);
			BoundingBox b2(target.nextPos().worldCoor(), target.nextPos().bounds_);
			return b1.isTouching(b2);
		}

		return true;
	}


	bool CollisionComponent
	::didGeometryCollide(const CollisionComponent& other) const {
		// Only cylinder support at the moment
		const Pos& pusher = posComponent().pos();
		const Pos& target = other.posComponent().pos();

		if(geometryType() == geom_CYLINDER && other.geometryType() == geom_CYLINDER) {
			BoundingCylinder b1(pusher.worldCoor(), pusher.bounds_);
			BoundingCylinder b2(pusher.worldCoor(), pusher.bounds_);
			return b2.isTouching(b1);
		}

		if(geometryType() == geom_BOX && other.geometryType() == geom_CYLINDER) {
			BoundingBox b1(pusher.worldCoor(), pusher.bounds_);
			BoundingCylinder b2(pusher.worldCoor(), pusher.bounds_);
			return b2.isTouching(b1);
		}

		if(geometryType() == geom_CYLINDER && other.geometryType() == geom_BOX) {
			BoundingCylinder b1(pusher.worldCoor(), pusher.bounds_);
			BoundingBox b2(target.worldCoor(), target.bounds_);
			return b1.isTouching(b2);
		}

		if(geometryType() == geom_BOX && other.geometryType() == geom_BOX) {
			BoundingBox b1(pusher.worldCoor(), pusher.bounds_);
			BoundingBox b2(target.worldCoor(), target.bounds_);
			return b1.isTouching(b2);
		}

		return true;
	}


	scale_t CollisionComponent
	::whenDoesGeometryCollide(const CollisionComponent& other) const {
		// Only cylinder support at the moment
		const PosComponent& pusher = posComponent();
		const PosComponent& target = other.posComponent();

		coor_t radiusSum = pusher.nextPos().radius() + target.nextPos().radius();
		coor_t radiusSumSq = radiusSum * radiusSum;

		scale_t min = 0, max = 1;
		Point3 mp, mt;
		for(int i = 0; i < 8; ++i) {
			scale_t middle = (min + max) / 2;
			pusher.worldCoor(middle, mp);
			target.worldCoor(middle, mt);

			coor_double_t dSq = mp.xzDistanceSquared(mt);
			if(dSq > radiusSumSq) {
				min = middle;
			}
			else {
				max = middle;
			}
		}
		return min;
	}


	int CollisionComponent
	::tag() const {
		return owner()->tag();
	}

	
	bool CollisionComponent 
	::shouldIgnore(const CollisionComponent& cc) const {
		return &cc == ignore_ || cc.ignore_ == this;
	}

}
