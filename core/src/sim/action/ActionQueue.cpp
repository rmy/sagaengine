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


#include "ActionQueue.hpp"
#include "../schema/SimSchema.hpp"
#include "ActionComponent.hpp"


namespace se_core {

	unsigned short ActionQueue
	::createActionChannelId() {
		static unsigned short idPool = 0;
		return idPool++;
	}


	ActionQueue
	::ActionQueue() :
		currentTurn_(0), currentInitiative_(0), isPerforming_(false)
		, scheduleCurrentTurn_(new ActorList::iterator_type[INITIATIVES_PER_TURN])
		, scheduleFutureTurns_(new ActorList::iterator_type[MAX_TURNS]) {
		channel_ = createActionChannelId();
		for(unsigned short i = 0; i < INITIATIVES_PER_TURN; ++i) {
			scheduleCurrentTurn_[ i ] = ActorList::end();
		}
		for(unsigned short i = 0; i < MAX_TURNS; ++i) {
			scheduleFutureTurns_[ i ] = ActorList::end();
		}
	}

	ActionQueue
	::~ActionQueue() {
		delete[] scheduleCurrentTurn_;
		delete[] scheduleFutureTurns_;
	}


	void ActionQueue
	::performScheduledActions(long when) {
		isPerforming_ = true;
		ActorList::iterator_type it = scheduleCurrentTurn_[ currentInitiative_ ];
		while(it != ActorList::end()) {
			ActionComponent* a = actorList_.next(it);
			a->perform(when, channel_);
		}
		isPerforming_ = false;
	}


	void ActionQueue
	::scheduleNextActions(long when) {
		ActorList::iterator_type it = scheduleCurrentTurn_[ currentInitiative_ ];
		while(it != ActorList::end()) {
			ActionComponent* a = actorList_.next(it);
			a->scheduleNextAction(when, channel_);
		}
	}


	unsigned short ActionQueue
	::add(ActionComponent &actor, short duration) {
		unsigned short turn = futureTurn(duration);

		if(turn == currentTurn_) {
			unsigned short initiative = futureInitiative(duration);
			actorList_.add(&actor, scheduleCurrentTurn_[ initiative ]);
		}
		else {
			actorList_.add(&actor, scheduleFutureTurns_[ turn ]);
		}

		return futureSchedule(duration);
	}


	bool ActionQueue
	::disrupt(ActionComponent &actor) {
		bool didDisrupt = false;
		unsigned short schedule = actor.actionSchedule(channel_);
		unsigned short turn = turnFromSchedule(schedule);
		if(turn == currentTurn_) {
			unsigned short initiative = initiativeFromSchedule(schedule);
			// Actions that are finished this initiative should be completed.
			// Makes most sense that an action should not be able to disrupt an
			// action in the same initiative, and also solves some hard
			// concurrency problems...
			if(!isPerforming_ || initiative != currentInitiative_) {
				actorList_.remove(&actor, scheduleCurrentTurn_[ initiative ]);
				didDisrupt = true;
			}
		}
		else {
			actorList_.remove(&actor, scheduleFutureTurns_[ turn ] );
			didDisrupt = true;
		}

		return didDisrupt;
	}


	void ActionQueue
	::beginNextInitiative() {
		actorList_.removeChain(scheduleCurrentTurn_[ currentInitiative_  ]);

		currentInitiative_ = futureInitiative(1);
		if(currentInitiative_ == 0) {
			beginNextTurn();
		}
	}


	void ActionQueue
	::beginNextTurn() {
		currentTurn_ = futureTurn(INITIATIVES_PER_TURN);

		ActorList::iterator_type it = scheduleFutureTurns_[ currentTurn_ ];
		while(it != ActorList::end()) {
			ActionComponent* a = actorList_.next(it);
			unsigned short initiative = initiativeFromSchedule(a->actionSchedule(channel_));
			actorList_.add(a, scheduleCurrentTurn_[ initiative ]);
		}
		actorList_.removeChain(scheduleFutureTurns_[ currentTurn_ ]);
	}


	void ActionQueue
	::reset() {
		actorList_.clear();
		currentTurn_ = 0;
		currentInitiative_ = 0;
		for(unsigned short i = 0; i < INITIATIVES_PER_TURN; ++i) {
			scheduleCurrentTurn_[ i ] = ActorList::end();
		}
		for(unsigned short i = 0; i < MAX_TURNS; ++i) {
			scheduleFutureTurns_[ i ] = ActorList::end();
		}
	}

}
