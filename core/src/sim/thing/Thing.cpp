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


#include "Thing.hpp"
#include "Actor.hpp"
#include "ThingManager.hpp"
#include "../config/all.hpp"
#include "../schema/SimSchema.hpp"
#include "../stat/SimObjectList.hpp"
#include "../stat/MultiSimObject.hpp"
#include "../area/Area.hpp"
#include "../area/sim_area.hpp"
#include "util/error/Log.hpp"
#include <cstring>



namespace se_core {

	DebugExec(int Thing::refCount = 0);

	Thing
	::Thing(const char* name)
			: PosNode(got_THING, name), spawner_(0)
			  , isPickable_(false), isCollideable_(false), isDead_(false) {
		DebugExec(++refCount);
	}


	Thing
	::Thing(enum SimObjectType type, const char* name)
			: PosNode(type, name), spawner_(0)
			  , isPickable_(false), isCollideable_(false), isDead_(false) {
		DebugExec(++refCount);
	}


	Thing
	::~Thing() {
		DebugExec(--refCount);
		//LogMsg(name());
	}


	void Thing
	::cleanup() {
		nextPos().resetParent();
		nextPos().resetArea();
		flip();
	}


	void Thing
	::scheduleForDestruction() {
		if(isDead_) return;

		//LogMsg(name());
		nextPos().resetArea();
		if(spawner()) {
			spawner()->decSpawnCount();
		}
		SimSchema::thingManager().scheduleForDestruction(*this);
		/*
		SimObjectList::iterator_type it = carriedThings().iterator();
		while(it != SimObjectList::NULL_NODE) {
			SimSchema::simObjectList.nextThing(it)->scheduleForDestruction();
			}
		*/
		isDead_ = true;
	}




	void Thing
	::setSpawner(Actor* spawner) {
		Assert(spawner != 0);
		spawner_ = spawner;
		spawner_->incSpawnCount();
	}



}
