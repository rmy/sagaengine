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


#ifndef ActionQueue_hpp
#define ActionQueue_hpp

#include "ActorList.hpp"
#include "../thing/sim_thing.hpp"


namespace se_core {

	class ActionQueue {
	public:
		ActionQueue();
		~ActionQueue();
		unsigned short add(Actor &actor, short duration);
		bool disrupt(Actor &actor);
		void scheduleNextActions(long when);
		void performScheduledActions(long when);
		void beginNextInitiative();
		void reset();


	private:
		void beginNextTurn();

		unsigned short futureInitiative(short duration) {
			return (currentInitiative_ + duration) & (INITIATIVES_PER_TURN - 1);
		}

		unsigned short futureSchedule(short duration) {
			return currentTurn_ * INITIATIVES_PER_TURN + currentInitiative_ + duration;
		}

		unsigned short futureTurn(short duration) {
			return (*this).turnFromSchedule((*this).futureSchedule(duration));
		}

		unsigned short turnFromSchedule(unsigned short schedule) {
			return (schedule >> INITIATIVES_PER_TURN_SHIFT) & (MAX_TURNS - 1);
		}

		unsigned short initiativeFromSchedule(unsigned short schedule) {
			return schedule & (INITIATIVES_PER_TURN - 1);
		}

		unsigned short createActionChannelId();

		static const unsigned short INITIATIVES_PER_TURN_SHIFT = 3; // Must be power of 2
		static const unsigned short INITIATIVES_PER_TURN = (1 << INITIATIVES_PER_TURN_SHIFT); // Must be power of 2
		static const unsigned short MAX_TURNS = 64; // Must be power of 2

		short currentTurn_;
		short currentInitiative_;
		short channel_;

		ActorList actorList_;
		ActorList::iterator_type* scheduleCurrentTurn_;
		ActorList::iterator_type* scheduleFutureTurns_;
	};

}

#endif
