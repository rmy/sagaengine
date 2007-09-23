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
#include "Anim.hpp"
#include "PosComponent.hpp"
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
			, index_(-1)
			, isGrounded_(true)
	{
		anim(0).setWeight(1);
		local_.setIdentity();
		world_.setIdentity();
	}


	short Pos
	::terrainStyle() const {
		if(!hasArea())
			return TS_VOID;
		const Area::Ptr a(area());
		return a->terrainStyle(this->localCoor(), index());
	}

	long Pos
	::touchedTerrain() const {
		if(!hasArea())
			return TSM_VOID;
		const Area::Ptr a(area());
		return a->touchedTerrain(this->localCoor(), radius());
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
		const Area::Ptr a(area());
		const Area::Ptr oth(other.area_);
		if(!(a->isNeighbour(*(oth))))
			return false;

		// All negative cases eliminated
		return true;
	}

	/*
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
			+ (coor_world_t)p.localCoor().x_;
		coor_world_t toX = (coor_world_t)area()->pagePosX()
			+ (coor_world_t)localCoor().x_;

		coor_world_t fromZ = ((coor_world_t)p.area()->pagePosZ()
				+ (coor_world_t)p.localCoor().z_);
		coor_world_t toZ = ((coor_world_t)area()->pagePosZ()
				+ (coor_world_t)localCoor().z_);

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
	*/


	void Pos
	::setPos(const Pos& original) {
		local_.setViewPoint(original.local_);
		world_.setViewPoint(original.world_);
		area_ = original.area_;
		parent_ = original.parent_;
		index_ = original.index_;
		isGrounded_ = original.isGrounded_;
		bounds_ = original.bounds_;
		for(int i = 0; i < MAX_ANIMS; ++i)
			anim_[i] = original.anim_[i];
	}


	void Pos
	::swapAnims(int a, int b) {
		Anim tmp(anim_[a]);
		anim_[a] = anim_[b];
		anim_[b] = tmp;
	}

	void Pos
	::setXZ(const Pos& original) {
		// Revert coordinates in xz plane. (Can still fall.)
		localCoor().x_ = original.localCoor().x_;
		localCoor().z_ = original.localCoor().z_;
		worldCoor().x_ = original.worldCoor().x_;
		worldCoor().z_ = original.worldCoor().z_;
		area_ = original.area_;
	}


	void Pos
	::resetParent(bool doKeepWorldCoor) {
		parent_ = area_;

		if(doKeepWorldCoor) {
			local_.setViewPoint(world_);
		}
		else {
			world_.setViewPoint(local_);
		}
	}


	bool Pos
	::didParentMove() const {
		return parent_ != 0 && parent_->didMove();
	}


	void Pos
	::setParent(PosComponent& p, bool doKeepWorldCoor) {
		parent_ = &p;
		if(doKeepWorldCoor) {
			local_.setViewPoint(world_);
			local_.sub(p.nextPos().world_);
		}
	}


	void Pos
	::setArea(PosComponent& area, bool doKeepWorldCoor) {
		//LogDetail(area.name());
		if(parent_ == area_) {
			// Area is the root parent node
			setParent(area, doKeepWorldCoor);
			//parent_ = &area;
		}
		area_ = &area;
		updateIndex();
	}


	void Pos
	::setArea(PosComponent& area, const ViewPoint& vp, bool isLocalViewPoint) {
		if(parent_ == area_) {
			// Area is the root parent node
			parent_ = &area;
		}
		area_ = &area;
		if(isLocalViewPoint) {
			local_.setViewPoint(vp);
			updateWorldViewPoint();
		}
		else {
			world_.setViewPoint(vp);
			updateLocalViewPoint();
		}
		updateIndex();
	}


	bool Pos
	::hasArea(Composite& area) const {
		return area_ != 0 && area_->owner() == &area; 
	}


	bool Pos
	::hasArea(Component& area) const {
		return area_ != 0 && area_->owner() == area.owner(); 
	}


	PosComponent* Pos
	::updateArea() {
		// Entered new area?
		Area::Ptr next(area_);
		if(!next->isLegalWorldCoor(worldCoor())) {
			Area* a = next->neighbour(worldCoor());
			if(a) {
				// Change area, keep world viewpoint
				setArea(*PosComponent::get(*a), true);
				next = a;
				// Cannot use old index as hint for index in new area
				setNoIndex();
			}
		}

		// Calc navigation mesh triangle id
		short newIndex = next->index(worldCoor(), index());
		setIndex(newIndex);
		return area_;
	}


	void Pos
	::updateIndex() {
		Area::Ptr next(area_);
		short newIndex = next->index(worldCoor(), -1);
		setIndex(newIndex);
	}


	void Pos
	::resetArea() {
		if(parent_ == area_) {
			parent_ = 0;
		}
		area_ = 0;
	}


	void Pos
	::updateWorldViewPoint() {
		world_.setViewPoint(local_);
		if(hasParent()) {
			// Parent should already have updated their worldViewPoint
			world_.add( parent()->nextPos().world_ );
		}
	}


	void Pos
	::areaCoor(const PosComponent& a, Point3& dest) const {
		dest.set( world_.coor_ );
		Assert(a.nextPos().worldFace().isIdentity());
		dest.sub( a.nextPos().world_.coor_ );
	}


	void Pos
	::areaFace(const PosComponent& a, Euler3& dest) const {
		dest.set( world_.face_ );
		Assert(a.nextPos().worldFace().isIdentity());
		dest.sub( a.nextPos().world_.face_ );
	}


	void Pos
	::areaViewPoint(const PosComponent& a, ViewPoint& dest) const {
		dest.setViewPoint(world_);
		dest.sub( a.nextPos().world_ );
	}

	void Pos
	::updateLocalViewPoint() {
		local_.setViewPoint(world_);
		if(hasParent()) {
			// Parent should already have updated their worldViewPoint
			local_.sub( parent()->nextPos().world_ );
		}
	}


	bool Pos
	::isInsideCollisionRange(const Pos& p) const {
		coor_double_t collisionRange = p.radius() + radius();
		return collisionRange * collisionRange >= worldCoor().distanceSquared(p.worldCoor());
	}


	bool Pos
	::isInsideCollisionRangeLinf(const Pos& p) const {
		coor_t collisionRange = p.radius() + radius();
		return worldCoor().xzDistanceLinf(p.worldCoor()) <= collisionRange;
	}


	bool Pos
	::hasInFront(const Point3& coor) const {
		// TODO:
		LogFatal("TODO:");
		return false; // (xzFaceAwayAngle(coor) < (BrayT::DEG180 >> 1));
	}


	void Pos
	::updateY() {
		if(isGrounded() && area_) {
			const Area::Ptr a(area());
			localCoor().y_ = a->groundHeight(localCoor(), index_);
		}
	}

}
