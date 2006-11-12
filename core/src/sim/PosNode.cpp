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
			: SimComposite(type, name)
			, isCollideable_(false)
			, didMove_(false)
			, spawnPoints_(0) {		
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
			setParent(*nextPos().parent());
			nextPos().parent()->addChildPosNode(*this);
		}
		else {
			resetParent();
			return false;
		}
		return true;
	}


	void PosNode
	::leaveCurrentArea() {
		if(position_.hasArea()) {
			//LogMsg(pos().worldCoor().toLog());
			//LogMsg(nextPos().worldCoor().toLog());
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
			// Non-movers are probably static
			// things or actors added to area.
			// Might as well freeze their coordinates
			// in world space, saving traversal overhead.

			node.nextPos().updateWorldViewPoint();
			// Freeze at world viewpoint
			node.nextPos().resetParent(true);
		}
	}


	void PosNode
	::removeChildPosNode(PosNode& node) {
		childPosNodes_.remove(node);
	}


	void PosNode
	::updateWorldViewPoint() {
		nextPosition_.world_.setViewPoint(nextPosition_.local_);
		if(nextPos().hasParent()) {
			nextPosition_.world_.add( nextPos().parent()->nextPos().world_ );
		}
	}


	void PosNode
	::worldCoor(scale_t alpha, Point3& dest) const {
		dest.set(position_.worldCoor());
		dest.interpolate(nextPosition_.worldCoor(), alpha);
	}


	void PosNode
	::worldViewPoint(scale_t alpha, ViewPoint& dest) const {
		dest.setViewPoint(position_.world_);
		dest.coor_.interpolate(nextPosition_.worldCoor(), alpha);
		dest.face_.interpolate(nextPosition_.worldFace(), alpha);
	}


	void PosNode
	::setSpawnPoints(int count, const ViewPoint* const* const spawnPoints) {
		if(spawnPoints_ != 0)
			LogFatal("Spawn points set twice: " << name());

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
