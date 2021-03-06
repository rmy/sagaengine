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
#include "SpawnManager.hpp"
#include "../action/all.hpp"
#include "../thing/Actor.hpp"
#include "../area/Area.hpp"
#include "util/type/String.hpp"
#include "util/error/Log.hpp"


namespace se_core {
	SpawnComponent
	::SpawnComponent(Composite* owner, PosComponent* pos)
		: Component(sct_SPAWN, owner), pos_(pos), spawnCount_(0), spawnPointCount_(0), spawnPoints_(0) {
	}

	SpawnComponent
	::SpawnComponent(Composite* owner, const ComponentFactory* factory)
			: Component(sct_SCRIPT, owner, factory)
			, spawnCount_(0), spawnPointCount_(0), spawnPoints_(0) {
		pos_ = static_cast<PosComponent*>(owner_->component(sct_POS));
		Assert(pos_);
	}


	SpawnComponent
	::~SpawnComponent() {
		cleanup();
	}


	void SpawnComponent
	::cleanup() {
		resetSpawner();
	}


	Composite* SpawnComponent
	::spawn(const char* thingName, int spawnPointId, long deniedTsMask) {
		// Get spawn point displace and face direction
		const ViewPoint* sp = spawnPoint(spawnPointId);
		AssertFatal(sp, "Spawn point does not exist");
		ViewPoint vp(*sp);

		// Calculate area coor of spawn point
		const Pos& pos = pos_->pos();
		vp.add(pos.world_);

		// Spawn it in area (with area as parent)
		Area::Ptr area(pos.area());
		area = area->neighbour(vp.coor_);
		if(!area) {
			LogWarning("Area not found");
			return 0;
		}
		PosComponent::Ptr aPos(*area);
		vp.sub(aPos->pos().world_);
		Composite* t = area->spawn(thingName, vp, deniedTsMask);

		// Remember spawner
		if(t) {
			SpawnComponent::Ptr tSpawn(*t);
			Assert(!tSpawn.isNull());
			tSpawn->setSpawner(owner());
		}

		return t;
	}

	void SpawnComponent
	::setSpawnPoints(int count, const ViewPoint* const* const spawnPoints) {
		AssertFatal(spawnPoints_ == 0, "Spawn points set twice: " << owner()->name());

		spawnPointCount_ = count;
		spawnPoints_ = spawnPoints;
	}


	bool SpawnComponent
	::hasSpawnPoint(short id) const {
		if(id < 0 || id >= spawnPointCount_)
			return false;
		return spawnPoints_[id] != 0;
	}

	const ViewPoint* SpawnComponent
	::spawnPoint(short id) const {
		AssertFatal(id >= 0 && id < spawnPointCount_, ": Id=" << id << " Count=" << spawnPointCount_ << " - " << owner()->name());
		AssertFatal(spawnPoints_[id] != 0, ": Id=" << id);
		return spawnPoints_[id];
	}


	Composite* SpawnComponent
	::spawner() const {
		if(spawner_.isNull()) {
			return 0;
		}
		return static_cast<Composite*>(spawner_.object());
	}


	void SpawnComponent
	::setSpawner(Composite* spawner) {
		Assert(spawner != 0);
		spawner_.set(spawner->ref());
		SpawnComponent::Ptr(*spawner)->incSpawnCount();
		Assert(!spawner_.isNull());
	}


	void SpawnComponent
	::resetSpawner() {
		if(!spawner_.isNull()) {
			Composite* s = static_cast<Composite*>(spawner_.object());
			SpawnComponent::Ptr(*s)->decSpawnCount();
			spawner_.reset();
		}
	}


	void SpawnComponent
	::setDead() {
		SpawnManager::singleton().scheduleForDestruction(*this->owner());
	}

}
