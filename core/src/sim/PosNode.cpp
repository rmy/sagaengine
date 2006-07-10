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
			, isCollideable_(false) {
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
			nextPos().updateY();
			nextPos().area()->addPosNode(*this);
		}
		else {
			return false;
		}
		return true;
	}



	void PosNode
	::addChildPosNode(PosNode& node) {
		// Only movers should be in the child pos tree.
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

			static Coor wc;
			nextWorldCoor(wc);
			nextPos().freezeAtWorldCoor(wc);
		}
	}


	void PosNode
	::removeChildPosNode(PosNode& node) {
		childPosNodes_.remove(node);
	}


	void PosNode
	::worldCoor(Coor& dest) const {
		dest.set(position_.coor_);
		const PosNode* node = position_.parent();
		while(node != 0) {
			dest.add(node->position_.coor_);
			node = node->position_.parent();
		}
	}


	void PosNode
	::worldViewPoint(ViewPoint& dest) const {
		dest.setViewPoint(position_);
		const PosNode* node = position_.parent();
		while(node != 0) {
			dest.coor_.add(node->position_.coor_);
			dest.face_.mul(node->position_.face_);
			node = node->position_.parent();
		}
	}


	void PosNode
	::worldCoor(scale_t alpha, Coor& dest) const {
		worldCoor(dest);
		static Coor np;
		nextWorldCoor(np);
		dest.interpolate(np, alpha);
	}


	void PosNode
	::worldViewPoint(scale_t alpha, ViewPoint& dest) const {
		worldViewPoint(dest);
		LogMsg(": " << dest.coor_.x_ << ", " << dest.coor_.y_ << ", " << dest.coor_.z_);
		static ViewPoint np;
		nextWorldViewPoint(np);
		LogMsg(": " << np.coor_.x_ << ", " << np.coor_.y_ << ", " << np.coor_.z_);
		dest.coor_.interpolate(np.coor_, alpha);
		dest.face_.slerp(np.face_, alpha, true);
		LogMsg(": " << dest.coor_.x_ << ", " << dest.coor_.y_ << ", " << dest.coor_.z_);
	}


	void PosNode
	::nextWorldCoor(Coor& dest) const {
		dest.set(nextPosition_.coor_);
		const PosNode* node = nextPosition_.parent();
		while(node != 0) {
			dest.add(node->nextPosition_.coor_);
			node = node->nextPosition_.parent();
		}
	}


	void PosNode
	::nextWorldViewPoint(ViewPoint& dest) const {
		dest.setViewPoint(nextPosition_);
		const PosNode* node = nextPosition_.parent();
		while(node != 0) {
			dest.coor_.add(node->nextPosition_.coor_);
			dest.face_.mul(node->nextPosition_.face_);
			node = node->nextPosition_.parent();
		}
	}


	void PosNode
	::setSpawnPoints(int count, SpawnPoint** spawnPoints) {
		LogMsg(name() << ": " << count);
		spawnPointCount_ = count;
		spawnPoints_ = spawnPoints;
	}


	SpawnPoint* PosNode
	::spawnPoint(short id) {
		LogMsg(name() << ": " << id);
		Assert(id >= 0 && id < spawnPointCount_);
		Assert(spawnPoints_[id] != 0);
		return spawnPoints_[id];
	}


}
