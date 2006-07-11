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
			, layer_(0)
	{
		face_.setIdentity();
		coor_.set(2.5, 0, 2.5);
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

		if(area_ != other.area_)
			return false;
		// Are pages neighbours?
		//if(!area_->isNeighbour(*other.area_))
		//	return false;

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
		area_ = original.area_;
		parent_ = original.parent_;
		layer_ = original.layer_;
		radius_ = original.radius_;
	}


	void Pos
	::resetParent() {
		parent_ = area_;
	}


	void Pos
	::setArea(Area& area) {
		//LogMsg(area.name());
		if(parent_ == area_) {
			// Area is the root parent node
			parent_ = &area;
		}
		area_ = &area;
	}

	#ifndef SE_EULER
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
	}
	#endif


	void Pos
	::setArea(Area& area, const Coor& c, const Euler3& a) {
		//LogMsg(area.name());
		if(parent_ == area_) {
			// Area is the root parent node
			parent_ = &area;
		}
		area_ = &area;
		setCoor(c);
		face_.setEuler(a);
	}


	void Pos
	::setArea(Area& area, const ViewPoint& vp) {
		if(parent_ == area_) {
			// Area is the root parent node
			parent_ = &area;
		}
		area_ = &area;
		setViewPoint(vp);
	}


	void Pos
	::setArea(Area& area, const SpawnPoint& sp) {
		if(parent_ == area_) {
			// Area is the root parent node
			parent_ = &area;
		}
		area_ = &area;
		setViewPoint(sp);
	}



	void Pos
	::resetArea() {
		if(parent_ == area_) {
			parent_ = 0;
		}
		area_ = 0;
	}


	void Pos
	::freezeAtWorldCoor(const Coor& c) {
		parent_ = 0;
		setCoor(c);
	}

	/*
	short Pos
	::clockwiseFaceDirection(short subdivisions) const {
		bray_t d = faceDirection() - (BrayT::DEG270 - (BrayT::DEG180 / subdivisions));
		return ((d & BRAY_MASK) / (BRAY_MASK / subdivisions));
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
		if(!hasOwnHeight() && area_) {
			coor_.y_ = area_->groundHeight(this->coor_);
		}
	}

}
