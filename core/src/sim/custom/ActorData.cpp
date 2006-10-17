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


#include "ActorData.hpp"
#include "ThingData.hpp"
#include "../sim.hpp"
#include "../config/all.hpp"
#include "../stat/all.hpp"


namespace se_core {

	ActorData
	::ActorData() {
	}


	coor_t ActorData
	::walkSpeed() const {
		return (abilities()->modified(Abilities::SPEED) * COOR_RES) / (coor_t)TIMESTEPS_PER_SECOND;
		//return (abilities()->modified(Abilities::SPEED) * COOR_RES) / (coor_t)TIMESTEP_INTERVAL;

		//return COOR_RES * CoorT::fromInt(abilities()->modified(Abilities::SPEED) >> TIMESTEP_INTERVAL);
		//return (((abilities()->modified(Abilities::SPEED) * COOR_RES) >> TIMESTEP_INTERVAL_SHIFT) * 3) >> 4;
	}


	void ActorData
	::setAbilities(short* abilities) {
	}


	void ActorData
	::setQuickMenuAction(const Action* a) {
		quickMenuAction_ = a;
	}


	void ActorData
	::setUseAction(const Action* a) {
		useAction_ = a;
	}

}
