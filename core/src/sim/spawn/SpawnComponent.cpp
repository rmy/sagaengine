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
		: SimComponent(sct_SPAWN, owner), pos_(pos), spawnCount_(0), spawnPointCount_(0), spawnPoints_(0) {
	}


	SpawnComponent
	::~SpawnComponent() {
		cleanup();
	}


	void SpawnComponent
	::cleanup() {
		resetSpawner();
	}


	SimComposite* SpawnComponent
	::spawn(const char* thingName, int spawnPointId, long deniedTsMask) {
		// Get spawn point displace and face direction
		const ViewPoint* sp = spawnPoint(spawnPointId);
		Assert(sp && "Spawn point does not exist");
		ViewPoint vp(*sp);

		// Calculate area coor of spawn point
		const PosNode* node = toActor();
		const Pos& pos = pos_->pos();
		vp.add(pos.world_);

		// Spawn it in area (with area as parent)
		Area* area = const_cast<Area*>(pos.area()->toArea());
		area = area->neighbour(vp.coor_);
		if(!area) {
			LogWarning("Area not found");
			return 0;
		}
		vp.sub(area->pos().world_);
		SimComposite* t = area->spawn(thingName, vp, deniedTsMask);

		// Avoid collision with spawner
		// TODO: Invent a better way
		//if(t) {
		//	t->setSpawner(toActor());
		//}
		if(t) static_cast<Actor*>(t)->setSpawner(toActor());

		return t;
	}


	void SpawnComponent
	::setSpawnPoints(int count, const ViewPoint* const* const spawnPoints) {
		AssertFatal(spawnPoints_ == 0, "Spawn points set twice: " << owner()->name());

		spawnPointCount_ = count;
		spawnPoints_ = spawnPoints;
	}


	const ViewPoint* SpawnComponent
	::spawnPoint(short id) const {
		AssertFatal(id >= 0 && id < spawnPointCount_, ": Id=" << id << " Count=" << spawnPointCount_);
		AssertFatal(spawnPoints_[id] != 0, ": Id=" << id);
		return spawnPoints_[id];
	}


	Actor* SpawnComponent
	::spawner() const {
		if(spawner_.isNull()) {
			return 0;
		}
		return static_cast<Actor*>(spawner_.object());
	}


	void SpawnComponent
	::setSpawner(Actor* spawner) {
		Assert(spawner != 0);
		spawner_.set(spawner->ptr());
		spawner->incSpawnCount();
		Assert(!spawner_.isNull());
	}


	void SpawnComponent
	::resetSpawner() {
		if(!spawner_.isNull()) {
			Actor* s = static_cast<Actor*>(spawner_.object());
			s->decSpawnCount();
			spawner_.reset();
		}
	}

}
