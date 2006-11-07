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


#include "SpawnComponent.hpp"
#include "../action/all.hpp"
#include "../thing/Actor.hpp"
#include "../area/Area.hpp"
#include "util/type/String.hpp"
#include "util/error/Log.hpp"


namespace se_core {
	SpawnComponent
	::SpawnComponent(SimComposite* owner, PosComponent* pos)
		: SimComponent(sct_SCRIPT, owner), pos_(pos), spawnCount_(0), spawnPointCount_(0) {
	}


	SpawnComponent
	::~SpawnComponent() {
		cleanup();
	}

	void SpawnComponent
	::cleanup() {
	}


	Thing* SpawnComponent
	::spawn(const char* thingName, int spawnPointId, long deniedTsMask) {
		// Get spawn point displace and face direction
		const ViewPoint* sp = spawnPoint(spawnPointId);
		Assert(sp && "Spawn point does not exist");
		ViewPoint vp(*sp);

		// Calculate area coor of spawn point
		const PosNode* node = owner();
		const Pos& pos = pos_->pos();
		vp.add(pos.world_);
		vp.sub(pos.area()->pos().world_);

		// Spawn it in area (with area as parent)
		Thing* t = const_cast<Area*>(pos.area())->spawn(thingName, vp, deniedTsMask);

		// Avoid collision with spawner
		if(t) t->setSpawner(owner());

		return t;
	}


	void SpawnComponent
	::setSpawnPoints(int count, const ViewPoint* const* const spawnPoints) {
		if(spawnPoints_ != 0)
			LogFatal("Spawn points set twice: " << owner()->name());

		spawnPointCount_ = count;
		spawnPoints_ = spawnPoints;
	}


	const ViewPoint* SpawnComponent
	::spawnPoint(short id) const {
		Assert(id >= 0 && id < spawnPointCount_);
		Assert(spawnPoints_[id] != 0);
		return spawnPoints_[id];
	}

}
