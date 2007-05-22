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


#include "ActionComponent.hpp"
#include "ActionAndParameter.hpp"
#include "../schema/SimSchema.hpp"


namespace se_core {

	ActionComponent
	::ActionComponent(Composite* owner)
		: Component(sct_ACTION, owner), feed_(0) {
		for(int i = 0; i < CHANNEL_COUNT; ++i) {
			presentActionScheduledComplete_[i] = 0;
		}
	}


	ActionComponent
	::~ActionComponent() {
	}


	void ActionComponent
	::cleanup() {
		for(int i = 0; i < CHANNEL_COUNT; ++i) {
			clearPlannedAction(i);
		}
		disrupt();
	}


	void ActionComponent
	::scheduleNextAction(short channel) {
		// Actions still in action queue after
		// scheduleForDestruction may try to plan
		// further actions
		if(isDead()) return;
		Assert(plannedAction_[channel].hasAction());
		presentAction_[channel] = plannedAction_[channel];
		plannedAction_[channel].resetAction();

		// Add to action queue
		const Action* a = presentAction_[channel].action();
		Parameter& p = presentAction_[channel].parameter();
		p.resetActionStage();
		presentActionScheduledComplete_[channel]
			= SimSchema::actionQueue[channel].add(*this, a->duration(*this, p));
		a->prepare(*this, p);
	}


	void ActionComponent
	::continueAction(long when, short channel) {
		// Add to action queue
		const Action* a = presentAction_[channel].action();
		Parameter& p = presentAction_[channel].parameter();
		p.incrActionStage();
		presentActionScheduledComplete_[channel]
			= SimSchema::actionQueue[channel].add(*this, a->duration(*this, p));
	}


	void ActionComponent
	::planAction(short channel, const Action& action, const Parameter* parameter) const {
		plannedAction_[channel].setAction(action);
		if(parameter) {
			plannedAction_[channel].copyParameter(*parameter);
		}
		if(!presentAction_[channel].hasAction() && plannedAction_[channel].hasAction()) {
			const_cast<ActionComponent&>(*this).scheduleNextAction(channel);
		}
	}


	void ActionComponent
	::planAction(short channel, const ActionAndParameter& action) const {
		if(!action.hasAction())
			return;
		plannedAction_[channel].set(action);
		if(!presentAction_[channel].hasAction() && plannedAction_[channel].hasAction()) {
			const_cast<ActionComponent&>(*this).scheduleNextAction(channel);
		}
	}


	void ActionComponent
	::planActionIfNone(short channel, const ActionAndParameter& action) const {
		if(!plannedAction_[ channel ].hasAction())
			planAction(channel, action);
	}


	void ActionComponent
	::clearPlannedAction(short channel) const {
		plannedAction_[channel].resetAction();
	}


	void ActionComponent
	::disrupt() {
		for(int i = 0; i < CHANNEL_COUNT; ++i) {
			disrupt(i);
		}
	}


	void ActionComponent
	::resume() {
		for(int i = 0; i < CHANNEL_COUNT; ++i) {
			presentAction_[ i ].setDisrupted(false);
			if(!presentAction_[ i ].hasAction()) {
				nextScriptAction(i);
				if(plannedAction_[i].hasAction()) {
					scheduleNextAction(i);
				}
			}
		}
	}


	bool ActionComponent
	::disrupt(short channel) {
		if(!presentAction_[ channel ].hasAction()) return true;
		// Actions are only removed from ActionQueue if it is in a future
		// initiative. If it is in the initiative presently performed, it
		// will not be touched...
		bool didDisrupt = SimSchema::actionQueue[ channel ].disrupt(*this);
		if(didDisrupt) {
			const Action* a = presentAction_[channel].action();
			Parameter& p = presentAction_[channel].parameter();
			a->disrupt(*this, p);
			presentAction_[channel].resetAction();
		}
		presentAction_[ channel ].setDisrupted(true);
		return didDisrupt;
	}


	void ActionComponent
	::nextScriptAction(short channel) {
		if(!feed_) return;
		ActionAndParameter& aap = plannedAction_[channel];
		feed_->nextAction(*this, channel, aap);
		if(aap.hasAction()) {
			if(!presentAction_[channel].hasAction()) {
				scheduleNextAction(channel);
			}
		}
	}


	void ActionComponent
	::setActionFeed(ActionFeed* feed) {
		Assert(feed_ == 0 &&  "Should only have one action feed");
		feed_ = feed;
	}


	void ActionComponent
	::setScriptActive(bool state) {
		if(state) {
			if(feed_) {
				for(int i = 0; i < CHANNEL_COUNT; ++i) {
					presentAction_[i].setDisrupted(false);
					nextScriptAction(i);
				}
			}
		}
		else {
			// Clear action in all channels
			for(int i = 0; i < CHANNEL_COUNT; ++i) {
				clearPlannedAction(i);
			}
			// Disrupt actions in progress
			disrupt();
		}
	}


	void ActionComponent
	::setActive(bool state) {
		setScriptActive(state);
	}
}
