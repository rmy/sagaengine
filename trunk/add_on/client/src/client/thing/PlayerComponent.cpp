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
		: SimComponent(sct_PLAYER, owner) {
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
			//	lastEntrance_.setViewPoint(owner_->nextPos().local_);
		}
	}


	void PlayerComponent
	::planAction(short channel, const Action& action, const Parameter* parameter) const {
		actionComponent_->planAction(channel, action, parameter);
	}


	void PlayerComponent
	::planDefaultMovementAction() const {
		if(defaultMovementAction_.hasAction()) {
			actionComponent_->planActionIfNone(CHANNEL_MOVEMENT, defaultMovementAction_);
		}

		if(defaultTurnAction_.hasAction()) {
			actionComponent_->planActionIfNone(CHANNEL_DIRECTION, defaultTurnAction_);
		}
	}


	void PlayerComponent
	::performDefaultMovementAction() const {
		long when = SimSchema::simEngine.when();
		if(defaultMovementAction_.hasAction()) {
			const Action* a = defaultMovementAction_.action();
			Parameter& p = defaultMovementAction_.parameter();
			LogMsg(a->name());
			a->perform(when, *actionComponent_, p);
		}
		if(defaultTurnAction_.hasAction()) {
			const Action* a = defaultTurnAction_.action();
			Parameter& p = defaultTurnAction_.parameter();
			a->perform(when, *actionComponent_, p);
		}
	}

}