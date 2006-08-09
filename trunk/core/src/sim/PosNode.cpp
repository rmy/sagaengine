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


#include "PosNode.hpp"
#include "./thing/Actor.hpp"
#include "./config/all.hpp"
#include "./schema/SimSchema.hpp"
#include "./stat/SimObjectList.hpp"
#include "./stat/MultiSimObject.hpp"
#include "./area/Area.hpp"
#include "./area/sim_area.hpp"
#include "util/error/Log.hpp"
#include <cstring>



namespace se_core {

	PosNode
	::PosNode(enum SimObjectType type, const char* name)
			: SimObject(type, name)
			, isCollideable_(false)
			, didMove_(false) {
	}


	PosNode
	::~PosNode() {
	}


	void PosNode
	::cleanup() {
		nextPos().resetParent();
		flip();
	}


	void PosNode
	::flip() {
		if(position_.parent() != nextPosition_.parent()) {
			changeParent();
		}
		if(position_.area() != nextPosition_.area()) {
			changeArea();
		}
		position_.setPos(nextPosition_);
		anim_ = nextAnim_;
		//LogMsg(name() << ": " << nextPosition_.toLog() << " - " << position_.toLog());
	}


	void PosNode
	::leaveCurrentParent() {
		if(position_.hasParent()) {
			position_.parent()->removeChildPosNode(*this);
		}
	}


	bool PosNode
	::changeParent() {
		leaveCurrentParent();

		if(nextPos().hasParent()) {
			nextPos().parent()->addChildPosNode(*this);
		}
		else {
			return false;
		}
		return true;
	}


	void PosNode
	::leaveCurrentArea() {
		if(position_.hasArea()) {
			position_.area()->removePosNode(*this);
		}
	}


	bool PosNode
	::changeArea() {
		if(position_.hasArea()) {
			leaveCurrentArea();
		}

		if(nextPos().hasArea()) {
			nextPos().area()->addPosNode(*this);
		}
		else {
			return false;
		}
		return true;
	}



	void PosNode
	::addChildPosNode(PosNode& node) {
		// Static pos nodes should have the area as
		// parent.
		if(node.isMover()) {
			// Only actors supported as movers
			Assert(node.isType(got_ACTOR));
			childPosNodes_.add(node);
		}
		else {
			// None movers are probably static
			// things or actors added to area.
			// Might as well freeze their coordinates
			// in world space, saving traversal overhead.

			//LogFatal("Not implemented");
			// CODE BELOW IS BUGGED! CALCULATES POS OF THIS INSTEAD OF CHILD
			//static ViewPoint vp;
			//nextWorldViewPoint(vp);
			node.nextPos().freezeAtWorldViewPoint();
		}
	}


	void PosNode
	::removeChildPosNode(PosNode& node) {
		childPosNodes_.remove(node);
	}


	void PosNode
	::updateWorldViewPoint() {
		ViewPoint& dest = nextPosition_.world_;
		dest.setViewPoint(nextPosition_.local_);
		if(nextPos().hasParent()) {
			// Parent should already have updated their worldViewPoint
			PosNode* p = nextPos().parent();
			const ViewPoint& src = p->nextPos().worldViewPoint();
			dest.coor_.rotate(src.face_);
			dest.coor_.add(src.coor_);
			dest.face_.rotate(src.face_);
		}
	}


	void PosNode
	::calcWorldViewPoint(ViewPoint& dest) const {
		dest.setViewPoint(nextPosition_.local_);
		if(nextPosition_.hasParent()) {
			// Parent should already have updated their worldViewPoint
			const PosNode* p = nextPosition_.parent();
			const ViewPoint& src = p->nextPosition_.world_;
			dest.coor_.rotate(src.face_);
			dest.coor_.add(src.coor_);
			dest.face_.rotate(src.face_);
		}
	}


	void PosNode
	::worldCoor(scale_t alpha, Point3& dest) const {
		/*
		worldCoor(dest);
		static Coor np; // WARNING: Not thread safe.
		nextWorldCoor(np);
		dest.interpolate(np, alpha);
		*/
		dest.set(position_.worldCoor());
		dest.interpolate(nextPosition_.worldCoor(), alpha);
	}



	void PosNode
	::worldViewPoint(scale_t alpha, ViewPoint& dest) const {
		/*
		worldViewPoint(dest);
		static ViewPoint np; // WARNING: Not thread safe.
		nextWorldViewPoint(np);
		*/
		dest.setViewPoint(position_.world_);
		dest.coor_.interpolate(nextPosition_.worldCoor(), alpha);
		dest.face_.interpolate(nextPosition_.worldFace(), alpha);
	}


	void PosNode
	::setSpawnPoints(int count, const ViewPoint* const* const spawnPoints) {
		//LogMsg(name() << ": " << count);
		spawnPointCount_ = count;
		spawnPoints_ = spawnPoints;
	}


	const ViewPoint* PosNode
	::spawnPoint(short id) const {
		Assert(id >= 0 && id < spawnPointCount_);
		Assert(spawnPoints_[id] != 0);
		return spawnPoints_[id];
	}


}