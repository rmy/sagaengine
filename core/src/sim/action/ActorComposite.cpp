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


#include "ActorComposite.hpp"
#include "ActionAndParameter.hpp"
#include "../schema/SimSchema.hpp"
#include "../stat/SortedSimObjectList.hpp"


namespace se_core {

	ActorComposite
	::ActorComposite(Actor* owner)
		: SimComposite(sct_ACTOR, owner), feed_(0) {
		//scriptStack_[currentScript_] = 0;
		for(int i = 0; i < CHANNEL_COUNT; ++i) {
			presentActionScheduledComplete_[i] = 0;
		}
	}


	ActorComposite
	::~ActorComposite() {
	}


	void ActorComposite
	::scheduleNextAction(short channel) {
		// Actions still in action queue after
		// scheduleForDestruction may try to plan
		// further actions
		if(isDead()) return;
		Assert(plannedAction_[channel].hasAction());
		//presentAction_[channel].setAction(*plannedAction_[channel].action());
		//presentAction_[channel].swapParameters(plannedAction_[channel]);
		presentAction_[channel] = plannedAction_[channel];
		plannedAction_[channel].resetAction();

		// Add to action queue
		const Action* a = presentAction_[channel].action();
		Parameter& p = presentAction_[channel].parameter();
		p.resetActionStage();
		presentActionScheduledComplete_[channel]
			= SimSchema::actionQueue[channel].add(*owner_, a->duration(*owner_, p));
		a->prepare(*owner_, p);
	}


	void ActorComposite
	::continueAction(long when, short channel) {
		// Add to action queue
		const Action* a = presentAction_[channel].action();
		Parameter& p = presentAction_[channel].parameter();
		p.incrActionStage();
		presentActionScheduledComplete_[channel]
			= SimSchema::actionQueue[channel].add(*owner_, a->duration(*owner_, p));
		a->prepare(*owner_, p);
	}


	void ActorComposite
	::planAction(short channel, const Action& action, const Parameter* parameter) const {
		plannedAction_[channel].setAction(action);
		if(parameter) {
			plannedAction_[channel].copyParameter(*parameter);
		}
		if(!presentAction_[channel].hasAction() && plannedAction_[channel].hasAction()) {
			const_cast<ActorComposite&>(*this).scheduleNextAction(channel);
		}
	}


	void ActorComposite
	::planAction(short channel, const ActionAndParameter& action) const {
		if(!action.hasAction())
			return;
		plannedAction_[channel].set(action);
		if(!presentAction_[channel].hasAction() && plannedAction_[channel].hasAction()) {
			const_cast<ActorComposite&>(*this).scheduleNextAction(channel);
		}
	}


	void ActorComposite
	::clearPlannedAction(short channel) const {
		plannedAction_[channel].resetAction();
	}


	void ActorComposite
	::disrupt() {
		for(int i = 0; i < CHANNEL_COUNT; ++i) {
			disrupt(i);
		}
	}


	bool ActorComposite
	::disrupt(short channel) {
		if(!presentAction_[ channel ].hasAction()) return true;
		// Actions are only removed from ActionQueue if it is in a future
		// initiative. If it is in the initiative presently performed, it
		// will not be touched...
		bool didDisrupt = SimSchema::actionQueue[ channel ].disrupt(*owner_);
		if(didDisrupt) {
			presentAction_[ channel ].resetAction();
		}
		return didDisrupt;
	}



	void ActorComposite
	::nextScriptAction(short channel) {
		if(!feed_) return;
		ActionAndParameter& aap = plannedAction_[channel];
		feed_->nextAction(*owner_, channel, aap);
		if(aap.hasAction()) {
			if(!presentAction_[channel].hasAction()) {
				scheduleNextAction(channel);
			}
		}
	}

}
