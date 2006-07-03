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


#include "WorldManager.hpp"
#include "../schema/NCursSchema.hpp"
#include "../thing/ThingEntity.hpp"
#include "../thing/ThingEntityList.hpp"
#include "util/error/Log.hpp"
#include "client/schema/ClientSchema.hpp"
#include "client/event/ClientListeners.hpp"
#include "sim/sim.hpp"
#include "sim/area/all.hpp"
#include "sim/schema/SimSchema.hpp"
#include "sim/stat/ThingIterator.hpp"
#include "sim/thing/Thing.hpp"
#include <cstring>

using namespace se_core;


namespace se_ncurs {
	WorldManager
	::WorldManager() {
		ClientSchema::clientListeners.addListener(*this);
	}


	WorldManager
	::~WorldManager() {
		ClientSchema::clientListeners.removeListener(*this);
		ThingEntityList::iterator_type it = NCursSchema::firstThingEntity;
		while(it != ThingEntityList::NULL_NODE) {
			ThingEntity* te = NCursSchema::thingEntityList.next(it);
			delete te;
		}
		NCursSchema::thingEntityList.removeChain(NCursSchema::firstThingEntity);
	}


	void WorldManager
	::setWorldGeometry(se_core::Area& area) {
	}


	void WorldManager
	::cameraEnteredAreaEvent(se_core::Area& area) {
		LogMsg(area.name());
		// Clear thinglist
		ThingEntityList::iterator_type it = NCursSchema::firstThingEntity;
		while(it != ThingEntityList::NULL_NODE) {
			ThingEntity* te = NCursSchema::thingEntityList.next(it);
			delete te;
		}
		NCursSchema::thingEntityList.removeChain(NCursSchema::firstThingEntity);

		// Add things
		se_core::ThingIterator nit(area.allThings());
		while(nit.hasNext()) {
			Thing& thing = nit.next();
			if(strncmp(thing.name(), "control.", strlen("control")) == 0) {
				LogMsg(thing.name());
				continue;
			}
			if(strncmp(thing.name(), "music.", strlen("music")) == 0) {
				LogMsg(thing.name());
				continue;
			}
			LogMsg(thing.name());
			ThingEntity* te = new ThingEntity(thing);
			NCursSchema::thingEntityList.add(*te, NCursSchema::firstThingEntity);
		}

		// Build terrain
		setWorldGeometry(area);
	}


	void WorldManager
	::cameraLeftAreaEvent(se_core::Area& area) {
		LogMsg(area.name());
		//areaMesh_->removeEntity();
		//delete areaMesh_;
	}


	void WorldManager
	::thingEnteredCameraAreaEvent(se_core::Thing& thing) {
		LogMsg(thing.name());

		if(strncmp(thing.name(), "control.", strlen("control")) == 0
		   || strncmp(thing.name(), "music.", strlen("music")) == 0
		   ) {
			LogMsg(thing.name());
			return;
		}

		// Add thing
		ThingEntity* te = new ThingEntity(thing);
		NCursSchema::thingEntityList.add(*te, NCursSchema::firstThingEntity);
	}


	void WorldManager
	::thingLeftCameraAreaEvent(se_core::Thing& thing) {
		LogMsg(thing.name());
		if(strncmp(thing.name(), "control.", strlen("control")) == 0
		   || strncmp(thing.name(), "music.", strlen("music")) == 0
		   ) {
			LogMsg(thing.name());
			return;
		}

		// Remove thing
		ThingEntityList::iterator_type it = NCursSchema::firstThingEntity;
		while(it != ThingEntityList::NULL_NODE) {
			LogMsg((sprintf(log_msg(), "Iterator: %d", it), log_msg()));
			ThingEntity* te = NCursSchema::thingEntityList.next(it);
			if(te->hasThing(thing)) {
				NCursSchema::thingEntityList.remove(*te, NCursSchema::firstThingEntity);
				delete te;
				break;
			}
		}
	}

}
