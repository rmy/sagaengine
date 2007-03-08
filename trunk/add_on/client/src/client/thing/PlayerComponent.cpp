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


#include "PlayerComponent.hpp"
#include "sim/sim.hpp"
#include "sim/SimEngine.hpp"
#include "sim/schema/SimSchema.hpp"
#include "sim/action/all.hpp"
#include "sim/thing/Actor.hpp"
#include "util/type/String.hpp"
#include "util/error/Log.hpp"

using namespace se_core;

namespace se_client {
	PlayerComponent
	::PlayerComponent(SimComposite* owner)
		: SimComponent(sct_PLAYER, owner), deadWhen_(0) {
		actionComponent_ = static_cast<ActionComponent*>(owner_->component(sct_ACTION));
		posComponent_ = static_cast<PosComponent*>(owner_->component(sct_POS));
	}


	PlayerComponent
	::~PlayerComponent() {
	}


	void PlayerComponent
	::setActive(bool state) {
	}
	

	void PlayerComponent
	::areaChanged(SimComposite* newArea, SimComposite* oldArea) {
		if(newArea) {
			PosComponent::Ptr pos(*newArea);
			lastEntrance_.setViewPoint(pos->nextPos().local_);
		}
	}


	void PlayerComponent
	::planAction(short channel, const Action& action, const Parameter* parameter) const {
		actionComponent_->planAction(channel, action, parameter);
	}


	bool PlayerComponent
	::die() {
		// Only die once
		if(deadWhen_)
			return false;
		deadWhen_ = SimSchema::simEngine.when();
		return true;
	}


	long PlayerComponent
	::deathAge() {
		if(!deadWhen_)
			return -1;

		return SimSchema::simEngine.when() - deadWhen_;
	}

}
