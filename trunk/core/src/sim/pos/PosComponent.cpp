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


#include "PosComponent.hpp"
#include "../thing/Actor.hpp"
#include "../config/all.hpp"
#include "../area/Area.hpp"
#include "util/error/Log.hpp"
#include <cstring>


namespace se_core {

	PosComponent
	::PosComponent(SimComposite* owner)
			: SimComponent(sct_POS, owner)
			, didMove_(false)
			, spawnPoints_(0) {		
	}


	PosComponent
	::~PosComponent() {
	}


	void PosComponent
	::flip() {
		if(position_.parent() != nextPosition_.parent()) {
			//TODO:
			//node_->changeParent();
		}
		if(position_.area() != nextPosition_.area()) {
			changeArea();
		}
		position_.setPos(nextPosition_);
	}


	void PosComponent
	::leaveCurrentArea() {
		if(position_.hasArea()) {
			//LogMsg(pos().worldCoor().toLog());
			//LogMsg(nextPos().worldCoor().toLog());
			position_.area()->removePosNode(*owner());
		}
	}


	bool PosComponent
	::changeArea() {
		if(position_.hasArea()) {
			leaveCurrentArea();
		}

		if(nextPos().hasArea()) {
			nextPos().area()->addPosNode(*owner());
		}
		else {
			return false;
		}
		return true;
	}


	void PosComponent
	::updateWorldViewPoint() {
		nextPosition_.world_.setViewPoint(nextPosition_.local_);
		if(nextPos().hasParent()) {
			nextPosition_.world_.add( nextPos().parent()->nextPos().world_ );
		}
	}


	void PosComponent
	::worldCoor(scale_t alpha, Point3& dest) const {
		dest.set(position_.worldCoor());
		dest.interpolate(nextPosition_.worldCoor(), alpha);
	}


	void PosComponent
	::worldViewPoint(scale_t alpha, ViewPoint& dest) const {
		dest.setViewPoint(position_.world_);
		dest.coor_.interpolate(nextPosition_.worldCoor(), alpha);
		dest.face_.interpolate(nextPosition_.worldFace(), alpha);
	}


	void PosComponent
	::setSpawnPoints(int count, const ViewPoint* const* const spawnPoints) {
		if(spawnPoints_ != 0)
			LogFatal("Spawn points set twice: " << owner()->name());

		spawnPointCount_ = count;
		spawnPoints_ = spawnPoints;
	}


	const ViewPoint* PosComponent
	::spawnPoint(short id) const {
		Assert(id >= 0 && id < spawnPointCount_);
		Assert(spawnPoints_[id] != 0);
		return spawnPoints_[id];
	}

}
