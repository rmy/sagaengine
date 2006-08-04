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


#include "Pos.hpp"
#include "Force.hpp"
#include "../PosNode.hpp"
#include "../area/Area.hpp"
#include "../sim.hpp"
#include "../schema/SimSchema.hpp"
#include "util/math/util_math.hpp"
#include "util/math/all.hpp"
#include <cstdio>
#include <cstring>


namespace se_core {

	Pos
	::Pos()	: area_(0)
			, parent_(0)
			, radius_(COOR_RES / 5)
			, index_(-1)
			, isGrounded_(true)
	{
		setIdentity();
		world_.setIdentity();
	}


	short Pos
	::terrainStyle() const {
		if(!hasArea())
			return TS_VOID;
		return area()->terrainStyle(this->coor_, index());
	}

	long Pos
	::touchedTerrain() const {
		if(!hasArea())
			return TSM_VOID;
		return area()->touchedTerrain(this->coor_, radius());
	}


	bool Pos
	::isKeyFramePath(const Pos& other) const {
		// Check that both positions are inside an area
		if(!hasArea() || !other.hasArea())
			return false;


		// Normal success case. Eliminate quickly.
		if(area_ == other.area_)
			return true;

		// Are pages not neighbours?
		if(!(area_->isNeighbour(*other.area_)))
			return false;

		// All negative cases eliminated
		return true;
	}


	coor_world_t Pos
	::pageDistanceSquared(const Pos& p) const {
		// Value returned if no legal path exists
		// TODO: Should perhaps be negative
		static const coor_world_t NO_PATH = -1;

		if(!hasArea() || !p.hasArea()) {
			// return error distance
			return NO_PATH;
		}
		if(!isKeyFramePath(p)) {
			// return error distance
			return NO_PATH;
		}
		coor_world_t fromX = (coor_world_t)p.area()->pagePosX()
			+ (coor_world_t)p.coor_.x_;
		coor_world_t toX = (coor_world_t)area()->pagePosX()
			+ (coor_world_t)coor_.x_;

		coor_world_t fromZ = ((coor_world_t)p.area()->pagePosZ()
				+ (coor_world_t)p.coor_.z_);
		coor_world_t toZ = ((coor_world_t)area()->pagePosZ()
				+ (coor_world_t)coor_.z_);

		coor_world_t xDist = fromX - toX;
		coor_world_t zDist = fromZ - toZ;

		//long xDist = (p.x_) - (x_);
		//long zDist = (p.z_) - (z_);

		coor_world_t xDistSquared = xDist * xDist;
		coor_world_t zDistSquared = zDist * zDist;
		coor_world_t distSquared = xDistSquared + zDistSquared;


#		ifdef SE_FIXED_POINT
		// Overflowing distances are clamped down
		// to this legal distance
		static const coor_world_t OVERFLOWED = -2;

		// Prevent overflow
		if(xDist > 0x7fff || xDist < -0x7fff || zDist > 0x7fff || zDist < -0x7fff)
			// Return greatest legal distance
			return OVERFLOWED;

		// May overflow on adding xDistSquared and yDistSquared
		if(distSquared < 0)
			return OVERFLOWED;
#		endif

		return distSquared;
	}


	void Pos
	::setPos(const Pos& original) {
		setViewPoint(original);
		world_.setViewPoint(original.world_);
		area_ = original.area_;
		parent_ = original.parent_;
		index_ = original.index_;
		isGrounded_ = original.isGrounded_;
		radius_ = original.radius_;
	}

	void Pos
	::setXZ(const Pos& original) {
		// Revert coordinates in xz plane. (Can still fall.)
		coor_.x_ = original.coor_.x_;
		coor_.z_ = original.coor_.z_;
		world_.coor_.x_ = original.world_.coor_.x_;
		world_.coor_.z_ = original.world_.coor_.z_;
		area_ = original.area_;
	}



	void Pos
	::resetParent() {
		parent_ = area_;
	}


	bool Pos
	::didParentMove() const {
		return parent_ != 0 && parent_->didMove();
	}

	void Pos
	::setParent(PosNode& p, bool doKeepWorldCoor) {
		parent_ = &p;
		if(!doKeepWorldCoor) {
			return;
		}

		setViewPoint(world_);
		sub(p.nextPos());
	}


	void Pos
	::setArea(Area& area, bool doKeepWorldCoor) {
		//LogMsg(area.name());
		if(parent_ == area_) {
			// Area is the root parent node
			setParent(area, doKeepWorldCoor);
			//parent_ = &area;
		}
		area_ = &area;
	}

	void Pos
	::setArea(Area& area, const Coor& c, const Quat4& q) {
		//LogMsg(area.name());
		if(parent_ == area_) {
			// Area is the root parent node
			parent_ = &area;
		}
		area_ = &area;
		setCoor(c);
		setFace(q);
		updateWorldViewPoint();
	}

	void Pos
	::setArea(Area& area, const Coor& c, const Euler3& a) {
		//LogMsg(area.name());
		if(parent_ == area_) {
			// Area is the root parent node
			parent_ = &area;
		}
		area_ = &area;
		coor_.set(c);
		face_.set(a);
		updateWorldViewPoint();
	}


	void Pos
	::setArea(Area& area, const ViewPoint& vp) {
		if(parent_ == area_) {
			// Area is the root parent node
			parent_ = &area;
		}
		area_ = &area;
		setViewPoint(vp);
		updateWorldViewPoint();
	}


	void Pos
	::setArea(Area& area, const SpawnPoint& sp) {
		if(parent_ == area_) {
			// Area is the root parent node
			parent_ = &area;
		}
		area_ = &area;
		setViewPoint(sp);
		updateWorldViewPoint();
	}


	void Pos
	::resetArea() {
		if(parent_ == area_) {
			parent_ = 0;
		}
		area_ = 0;
	}


	void Pos
	::freezeAtWorldViewPoint() {
		updateWorldViewPoint();
		parent_ = 0;
		setViewPoint(world_);
	}

	void Pos
	::updateWorldViewPoint() {
		world_.setViewPoint(*this);
		if(hasParent()) {
			// Parent should already have updated their worldViewPoint
			world_.add( parent()->nextPos().world_ );
		}
	}


	void Pos
	::updateLocalViewPoint() {
		setViewPoint(world_);
		if(hasParent()) {
			// Parent should already have updated their worldViewPoint
			world_.sub( parent()->nextPos().world_ );
		}
	}

	/*
	short Pos
	::clockwiseFaceDirection(short subdivisions) const {
		bray_t d = faceDirection() - (BrayT::DEG270 - (BrayT::DEG180 / subdivisions));
		return ((d & BRAY_MASK) / (BRAY_MASK / subdivisions));
		}
	*/

	void Pos
	::worldViewPoint(ViewPoint& dest, bool doCalcNext, const PosNode* stopAt) const {
		dest.setViewPoint(*this);
		const PosNode* node = parent();
		while(node != 0 && node != stopAt) {
			const Pos& nodePos = node->pos();

			dest.coor_.rotate(nodePos.face_);
			dest.coor_.add(nodePos.coor_);
			dest.face_.rotate(nodePos.face_);
			node = nodePos.parent();
		}
	}


	/*
	void Pos
	::nextWorldViewPoint(ViewPoint& dest, const PosNode* stopAt) const {
		dest.setViewPoint(*this);
		const PosNode* node = parent();
		while(node != 0 && node != stopAt) {
			dest.coor_.rotate(node->nextPos().face_);
			dest.coor_.add(node->nextPos().coor_);
			dest.face_.rotate(node->nextPos().face_);
			node = node->nextPos().parent();
		}
	}


	void Pos
	::worldCoor(Coor& dest, const PosNode* stopAt) const {
		dest.set(this->coor_);
		const PosNode* node = parent();
		while(node != 0 && node != stopAt) {
			dest.rotate(node->pos().face_);
			dest.add(node->pos().coor_);
			node = node->pos().parent();
		}
	}
	*/



	bool Pos
	::isInsideCollisionRange(const Pos& p) const {
		coor_double_t collisionRange = p.radius() + radius();
		return collisionRange * collisionRange >= coor_.distanceSquared(p.coor_);
	}


	bool Pos
	::isInsideCollisionRangeLinf(const Pos& p) const {
		coor_t collisionRange = p.radius() + radius();
		return coor_.xzDistanceLinf(p.coor_) <= collisionRange;
	}


	/*
	bray_t Pos
	::xzFaceAwayAngle(const Point3& coor) const {
		bray_t a = ((xzAngleTowards(coor) - faceDirection()) & BRAY_MASK);
		if(a >= BrayT::DEG180) a = BRAY_RANGE - a;
		return a;
		}
	*/


	bool Pos
	::hasInFront(const Point3& coor) const {
		// TODO:
		LogFatal("TODO:");
		return false; // (xzFaceAwayAngle(coor) < (BrayT::DEG180 >> 1));
	}


	void Pos
	::updateY() {
		if(isGrounded() && area_) {
			coor_.y_ = area_->groundHeight(this->coor_);
		}
	}

}
