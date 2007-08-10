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


#include "Spawn.hpp"
#include "sim/schema/SimSchema.hpp"
#include "sim/pos/Pos.hpp"
#include "sim/thing/Actor.hpp"
#include "sim/spawn/SpawnComponent.hpp"

using namespace se_core;

namespace se_basic {
	Spawn
	::Spawn()
		: se_core::Action("Spawn") 
		, PROPERTY_SPAWN(Property::hash("Action.SPAWN")) 
		, PROPERTY_SPAWN_POINT(Property::hash("Action.SPAWN_POINT")) 
		, PROPERTY_SPAWN_MAX(Property::hash("Action.SPAWN_MAX")) 
	{
	}


	void Spawn
	::perform(long when, ActionComponent& perf, Parameter& parameter) const {
		StatComponent::Ptr pStat(perf);
		SpawnComponent::Ptr pSpawn(perf);
		const Property* prop = pStat->property(PROPERTY_SPAWN);
		if(prop) {
			const Property* propPoint = pStat->property(PROPERTY_SPAWN_POINT);
			const Property* propMax = pStat->property(PROPERTY_SPAWN_MAX);
			LogWarning(propMax->shortValue() << " < " << pSpawn->spawnCount());

			if(!propMax || propMax->shortValue() > pSpawn->spawnCount()) {
				pSpawn->spawn(prop->string(), propPoint->shortValue());
			}

		}
		else {
			Param* p = static_cast<Param*>(parameter.data(sizeof(Param)));
			pSpawn->spawn(p->thingType_, p->spawnPoint_);
		}
	}


	const Spawn& Spawn
	::param(const char* thingType, short spawnPoint, Parameter& out) const {
		Param* p = static_cast<Param*>(out.data(sizeof(Param)));
		*p = Param(thingType, spawnPoint);
		return *this;
	}


	const Spawn actionSpawn;
}

