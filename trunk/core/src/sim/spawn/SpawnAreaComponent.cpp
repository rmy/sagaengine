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


#include "SpawnManager.hpp"
#include "SpawnAreaComponent.hpp"
#include "SpawnComponent.hpp"
#include "sim/sim.hpp"
#include "sim/schema/SimSchema.hpp"
#include "comp/list/CompositeList.hpp"
#include "util/error/Log.hpp"
#include "comp/schema/CompSchema.hpp"
#include "../area/Area.hpp"



namespace se_core {

	SpawnAreaComponent
	::SpawnAreaComponent(Composite* owner, const SimComponentFactory* factory) 
			: AreaComponent(sct_SPAWN, owner, factory), spawnPointCount_(0), spawnPoints_(0) {
	}


	SpawnAreaComponent
	::~SpawnAreaComponent() {
	}


	void SpawnAreaComponent
	::setActive(bool state) {
		if(state) {
			SimNodeComponent* c = static_cast<SimNodeComponent*>(CompSchema::activeRoot().component(type_));
			if(c) {
				setParent(*c);
			}
		}
		else {
			resetParent();
		}
	}


	void SpawnAreaComponent
	::flipSpawns(void) {
		// Flip new spawns into area
		CompositeList::Iterator it(newSpawns_);
		while(it.hasNext()) {
			// Spawned things doesn't have to be actors
			Composite& c = it.next();

			// Newly spawned things will change area on first flip.
			// This will call Area::addThing, inserting it into
			// collisionGrid_ and other structures where it belong
			PosComponent::Ptr(c)->flip();
		}

		// New spawn are no longer new spawns once they are flipped
		newSpawns_.clear();
	}



	SimComposite* SpawnAreaComponent
	::spawn(const char* thingName, const ViewPoint& vp, PosComponent* parent) {
		/*
		if(deniedTsMask != 0 && (tsMask(terrainStyle(vp.coor_)) & deniedTsMask) != 0) {
			// Tried to spawn on denied terrain type
			LogMsg((int)(tsMask(terrainStyle(vp.coor_))));
			return 0;
		}
		*/

		// Create the thing
		SimComposite* thing = SimSchema::spawnManager().create(thingName);
		PosComponent::Ptr sPos(thing);
		Assert(thing && !sPos.isNull());

		// Set position and direction
		PosComponent::Ptr aPos(*this);
		sPos->nextPos().setArea(*aPos, vp);

		if(parent) {
			sPos->nextPos().setParent(*parent);
		}

		// Initial index, if area type is using it
		sPos->nextPos().updateWorldViewPoint();
		sPos->nextPos().setIndex( toArea()->index(sPos->nextPos().worldCoor()) );

		// Add the thing to the list of new spawns
		newSpawns_.add(*thing);

		// Return the newly created thing
		return thing;
	}



	void SpawnAreaComponent
	::setSpawnPoints(int count, const ViewPoint* const* const spawnPoints) {
		AssertFatal(spawnPoints_ == 0, "Spawn points set twice: " << owner()->name());

		spawnPointCount_ = count;
		spawnPoints_ = spawnPoints;
	}


	const ViewPoint* SpawnAreaComponent
	::spawnPoint(short id) const {
		AssertFatal(id >= 0 && id < spawnPointCount_, ": Id=" << id << " Count=" << spawnPointCount_);
		AssertFatal(spawnPoints_[id] != 0, ": Id=" << id);
		return spawnPoints_[id];
	}

}

