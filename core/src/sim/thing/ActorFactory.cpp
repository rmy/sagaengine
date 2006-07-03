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


#include "ActorFactory.hpp"
#include "Thing.hpp"
#include "Actor.hpp"
#include "../sim.hpp"


namespace se_core {

	ActorFactory
	::ActorFactory(String* name) : ThingFactory(got_ACTOR, name) {
	}


	ActorFactory
	::~ActorFactory() {
	}


	Thing* ActorFactory
	::create() const {
		Thing* t;
		Actor* a;
		t = a = new Actor(name_->get());
		/*
		  a->setPickable(isPickable_);
		  a->setQuickMenuAction(quickMenuAction_);
		  a->setUseAction(useAction_);
		  a->abilities()->setBases(abilities_);
		  a->setDefaultPhysics(physics_);
		  a->setCollide(collide_);

		t->nextPos().setRadius(radius_);
		t->nextPos().changeDirection(singleValues_[SV_DIRECTION]);
		t->nextPos().setFaceDirection(singleValues_[SV_DIRECTION]);
		//LogMsg((sprintf(log_msg(), "%s: %d", t->name(), singleValues_[SV_DIRECTION]), log_msg()));

		for(int i = 0; i < SV_COUNT; ++i) {
			if(!singleValues_[i]) continue;
			t->setSingleValue(i, singleValues_[i]);
			//printf("Setting single value: %d, %d\n", i, singleValues[i]);
		}
		for(int i = 0; i < MGO_COUNT; ++i) {
			if(!multiSimObjects_[i]) continue;
			t->multiSimObject(i).add(*multiSimObjects_[i]);
		}
		if(script_) {
		a->setDefaultScript(SimSchema::sortedSimObjectList().script(script_->get()));
			}
		*/

		return t;
	}

}

