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
#include "sim_action.hpp"
#include "../thing/sim_thing.hpp"


namespace se_core {

	/**
	 * The action queue stores who is about to perform actions.
	 * You will not access this class directly. You plan actions
	 * using ActionComponent::planAction(...), and disrupt them by calling
	 * ActionComponent::disrupt(...)
	 */
	class _SeCoreExport ActionQueue {
	public:
		ActionQueue();
		~ActionQueue();
		/**
		 * Add and actor that plans to perform an action. The
		 * ActionQueue does not store the Actions themselves, but
		 * instead the actor that is to perform them. When the
		 * action is performed, the ActionComponent is requested by
		 * the ActionQueue to perform the planned Action.
		 */
		unsigned short add(ActionComponent &actor, short duration);

		/**
		 * Remove an ActionComponent from the action queue.
		 */
		bool disrupt(ActionComponent &actor);

		/**
		 * Request ActionComponents that has just perfomed an action
		 * to schedule their next action.
		 */
		void scheduleNextActions(long when);

		/**
		 * Ask actors that are scheduled to act this
		 * timestep to perform their actions.
		 */
		void performScheduledActions(long when);

		/**
		 * Prepare for next timestep.
		 */
		void beginNextInitiative();

		/**
		 * Empty the action queue.
		 */
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

		bool isPerforming_;

		ActorList actorList_;
		ActorList::iterator_type* scheduleCurrentTurn_;
		ActorList::iterator_type* scheduleFutureTurns_;
	};

}

#endif
