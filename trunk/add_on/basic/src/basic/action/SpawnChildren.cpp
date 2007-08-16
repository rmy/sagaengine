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


#include "SpawnChildren.hpp"
#include "sim/schema/SimSchema.hpp"
#include "sim/pos/Pos.hpp"
#include "sim/thing/Actor.hpp"
#include "sim/spawn/SpawnComponent.hpp"
#include "sim/pos/PosComponent.hpp"
#include "sim/physics/PhysicsComponent.hpp"

using namespace se_core;

namespace se_basic {
	SpawnChildren
	::SpawnChildren()
		: se_core::Action("SpawnChildren") 
		, PROPERTY_SPAWN(Property::hash("Action.SPAWN_CHILDREN")) 
		, PROPERTY_SPAWN_POINT(Property::hash("Action.SPAWN_CHILDREN_POINT")) 
	{
	}


	void SpawnChildren
	::perform(long when, ActionComponent& perf, Parameter& parameter) const {
		StatComponent::Ptr pStat(perf);
		SpawnComponent::Ptr pSpawn(perf);
		const Property* prop = pStat->property(PROPERTY_SPAWN);
		if(prop) {
			PosComponent::Ptr pos(perf);
			const Property* propPoint = pStat->property(PROPERTY_SPAWN_POINT);
			for(unsigned int i = 0; i < prop->valueCount(); ++i) {
				Composite* s = pSpawn->spawn(prop->string(i), propPoint->shortValue() + i);
				PosComponent::Ptr childPos(s);
				if(!childPos.isNull() && !pos.isNull()) {
					PosComponent::Ptr(*s)->nextPos().setParent(*pos, true);
				}
			}

		}
		else {
			Param* p = static_cast<Param*>(parameter.data(sizeof(Param)));
			pSpawn->spawn(p->thingType_, p->SpawnChildrenPoint_);
		}
	}


	const SpawnChildren& SpawnChildren
	::param(const char* thingType, short SpawnChildrenPoint, Parameter& out) const {
		Param* p = static_cast<Param*>(out.data(sizeof(Param)));
		*p = Param(thingType, SpawnChildrenPoint);
		return *this;
	}


	const SpawnChildren actionSpawnChildren;
}

