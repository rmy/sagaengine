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
#include "util/error/Log.hpp"
#include "../thing/Actor.hpp"
#include "../react/all.hpp"
#include "util/bounds/BoundingBox.hpp"
#include "util/bounds/BoundingCylinder.hpp"


namespace se_core {
	/*
	CollisionComponent
	::CollisionComponent(Composite* owner, PosComponent* posComponent)
			: AreaChildComponent(sct_COLLISION, owner)
			, posComponent_(posComponent)
			, isCollideable_(false)
			, ignore_(0), p1_(0, 0, 0), p2_(0, 0, 0), radius_(0) {
		geometryType_ = geometryType();
	}
	*/


	CollisionComponent
	::CollisionComponent(Composite* owner, const ComponentFactory* factory)
			: AreaChildComponent(sct_COLLISION, owner, factory)
			, isCollideable_(false)
			, ignore_(0), p1_(0, 0, 0), p2_(0, 0, 0), radius_(0) {
		posComponent_ = static_cast<PosComponent*>(owner_->component(sct_POS));
		Assert(posComponent_);
		geometryType_ = geometryType();
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
	::zoneChanged(int zoneType, Composite* newArea, Composite* oldArea) {
		if(zoneType != st_AREA)
			return;

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
		geometryType_ = geometryType();
		if(geometryType_ == geom_CYLINDER) {
			p1_.reset();
			p2_.reset();
			radius_ = posComponent_->nextPos().bounds_.radius();
			BoundingBox toBox(posComponent_->nextPos().worldCoor(), posComponent_->nextPos().bounds_);
			areaCovered_ = toBox;
			if(posComponent_->pos().isKeyFramePath(posComponent_->nextPos())) {
				BoundingBox fromBox(posComponent_->pos().worldCoor(), posComponent_->pos().bounds_);
				areaCovered_.merge(fromBox);
			}
		}
		else if(geometryType_ == geom_LONG_CYLINDER) {
			const BoundingBox& b = posComponent().nextPos().bounds_;
			coor_t xSize = b.maxX_ - b.minX_;
			coor_t zSize = b.maxZ_ - b.minZ_;
			if(xSize > zSize) {
	 			radius_ = CoorT::half(zSize);
				p1_.z_ = p2_.z_ = CoorT::half(b.minZ_ + b.maxZ_);
				p1_.x_ = b.minX_ + radius_;
				p2_.x_ = b.maxX_ - radius_;
			}
			else {
				radius_ = CoorT::half(xSize);
				p1_.x_ = p2_.x_ = CoorT::half(b.minX_ + b.maxX_);
				p1_.z_ = b.minZ_ + radius_;
				p2_.z_ = b.maxZ_ - radius_;
			}
			p1_.y_ = p2_.y_ = b.minY_;

			Euler3& face = posComponent_->nextPos().worldFace();
			if(!face.isIdentity()) {
				AssertWarning(face.pitch_ == 0 || face.roll_ == 0, "Pitch and roll not supported for non-cylindrical things");
				if(face.pitch_ != 0 || face.roll_ != 0) {
					setCollideable(false);
				}
				p1_.rotate(face);
				p2_.rotate(face);
			}

			Point3 wp1, wp2;
			wp1.add(p1_, posComponent_->nextPos().worldCoor());
			wp2.add(p2_, posComponent_->nextPos().worldCoor());

			coor_t height = b.maxY_ - b.minY_;

			BoundingBox b1(wp1, radius_, height);
			BoundingBox b2(wp2, radius_, height);
			areaCovered_ = b1;
			areaCovered_.merge(b2);
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


	float CollisionComponent
	::penetration(const CollisionComponent& other) const {
		Point3 p, t;
		coor_t radSum = bouncePoints(SCALE_RES, other, p, t);
		float pen = radSum - CoorT::sqrt(p.xzDistanceSquared(t));
		if(pen < 0)
			return 0;
		return pen;
	}


	bool CollisionComponent
	::doesGeometryCollide(const CollisionComponent& other) const {
		Point3 p, t;
		coor_t radSum = bouncePoints(SCALE_RES, other, p, t);
		bool res = (p.xzDistanceSquared(t) < radSum * radSum);
		return res;
	}


	bool CollisionComponent
	::didGeometryCollide(const CollisionComponent& other) const {
		Point3 p, t;
		coor_t radSum = bouncePoints(0, other, p, t);
		return (p.xzDistanceSquared(t) < radSum * radSum);
	}


	scale_t CollisionComponent
	::whenDoesGeometryCollide(const CollisionComponent& other) const {
		// Only cylinder support at the moment
		const PosComponent& pusher = posComponent();
		const PosComponent& target = other.posComponent();

		coor_t radiusSum = pusher.nextPos().bounds_.smallRadius() + target.nextPos().bounds_.smallRadius();
		coor_t radiusSumSq = radiusSum * radiusSum;

		Point3 tNow, tNext;

		scale_t min = 0, max = 1;
		Point3 mp, mt;
		for(int i = 0; i < 8; ++i) {
			scale_t middle = (min + max) / 2;
			pusher.worldCoor(middle, mp);
			//target.worldCoor(middle, mt);
			other.bouncePoint(middle, mp, mt);

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


	coor_t CollisionComponent
	::bouncePoint(scale_t alpha, const Point3& testPoint, Point3& dest) const {
		Point3 c;
		posComponent().worldCoor(alpha, c);

		if(geometryType_ == geom_CYLINDER) {
			dest.set(c);
			return radius_;
		}

		/*
		const BoundingBox& b = posComponent().pos().bounds_;
		coor_t xSize = b.maxX_ - b.minX_;
		coor_t zSize = b.maxZ_ - b.minZ_;

		Point3 p1, p2;
		coor_t radius;
		if(xSize > zSize) {
 			radius = CoorT::half(zSize);
			p1.z_ = p2.z_ = CoorT::half(b.minZ_ + b.maxZ_);
			p1.x_ = b.minX_ + radius;
			p2.x_ = b.maxX_ - radius;

			Assert(p1.x_ < p2.x_);
		}
		else if(zSize > xSize) {
			radius = CoorT::half(xSize);
			p1.x_ = p2.x_ = CoorT::half(b.minX_ + b.maxX_);
			p1.z_ = b.minZ_ + radius;
			p2.z_ = b.maxZ_ - radius;
			Assert(p1.z_ < p2.z_);
		}
		else {
			dest.set(c);
			return CoorT::half(xSize);
		}

		p1.y_ = p2.y_ = CoorT::half(b.minY_ + b.maxY_);
		Assert(posComponent_->nextPos().worldFace().pitch_ == 0);
		Assert(posComponent_->nextPos().worldFace().roll_ == 0);
		p1.rotate(posComponent().nextPos().worldFace());
		p2.rotate(posComponent().nextPos().worldFace());
		*/

		Point3 wp1, wp2;
		wp1.add(c, p1_);
		wp2.add(c, p2_);
		dest.nearestPoint(wp1, wp2, testPoint);
		return radius_;
	}


	coor_t CollisionComponent
	::bouncePoints(scale_t alpha, const CollisionComponent& other, Point3& d1, Point3& d2) const {
		Point3 tmp;
		other.posComponent().worldCoor(alpha, tmp);
		coor_t radSum = bouncePoint(alpha, tmp, d1);
		radSum += other.bouncePoint(alpha, d1, d2);
		d1.y_ = d2.y_;
		return radSum;
	}

	bool CollisionComponent 
	::shouldIgnore(const CollisionComponent& cc) const {
		return &cc == ignore_ || cc.ignore_ == this;
	}
}
