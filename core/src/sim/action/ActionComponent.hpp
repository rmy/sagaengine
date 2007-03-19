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


#ifndef ActionComponent_hpp
#define ActionComponent_hpp

#include "sim_action.hpp"
#include "../SimComponent.hpp"
#include "../SimComposite.hpp"
#include "../sim.hpp"
#include "Action.hpp"
#include "ActionQueue.hpp"
#include "ActionAndParameter.hpp"
#include "ActionFeed.hpp"

namespace se_core {

	/**
	 * Base class for actions.
	 * ActionComponents are the atoms of game-character behaviour in SagaEngine.
	 * They define what a character can do.
	 */
	class _SeCoreExport ActionComponent : public SimComponent {
	public:
		typedef Ptr<ActionComponent, sct_ACTION> Ptr;

		/** Constructor.
		 */
		ActionComponent(Composite* owner);
		~ActionComponent();


		static ActionComponent* get(Composite& composite) {
			ActionComponent* c = static_cast<ActionComponent*>(composite.component(se_core::sct_ACTION));
			return c;
		}

		static ActionComponent* get(SimComponent& component) {
			ActionComponent* c = static_cast<ActionComponent*>(component.owner()->component(se_core::sct_ACTION));
			return c;
		}



		/**
		 * Plan a new action.
		 * Plan an Action that will be scheduled when the present one is
		 * finished. Will overwrite any action that is presently planned
		 * but not yet in the ActionQueue.
		 */
		void planAction(short channel, const Action& action, const Parameter* parameter = 0) const;
		void planAction(short channel, const ActionAndParameter& action) const;
		void planActionIfNone(short channel, const ActionAndParameter& action) const;

		/**
		 * Clear planned action.
		 * Clear an action that is waiting for being entered into the
		 * ActionQueue, but hasn't yet gotten there (because another
		 * Action for the same ActionComponent and channel is already in the queue).
		 */
		void clearPlannedAction(short channel) const;

		/**
		 * Disrupt all actions the this ActionComponent has in the ActionQueue.
		 * Actions that are performed in the on-going initiative are not
		 * disrupted.
		 */
		void disrupt();

		void resume();

		/**
		 * Disrupt an action the this ActionComponent has in a given ActionQueue channel.
		 * Actions that are performed in the on-going initiative are not
		 * disrupted.
		 */
		bool disrupt(short channel);

		/**
		 * Schedule this ActionComponent for destruction.
		 * It will be destroyed when the on-going initative is performed,
		 * allowing the actor to complete any action it is performing this
		 * initiative.
		 */
		void scheduleForDestruction();

		void resetActionFeed() {
			feed_ = 0;
		}

		void setActionFeed(ActionFeed* feed);

		void setActive(bool state);
		void setScriptActive(bool state);

	protected:
		friend class ActionQueue;

		void cleanup();

		/**
		 * Perform an already scheduled Action.
		 * Don't call this method yourself. It is used by the ActionQueue.
		 */
		inline void perform(long when, short channel);

		/**
		 * Schedule next action.
		 * Schedule the next action when the present one is finished. The
		 * next action may be:
		 * - The same one if it is a multi-stage action  that has not finished it's last stage
		 * - It can be a planned one.
		 * - The same one if it is auto-repeating and no new action is planned.
		 * - One fetched from the present Script if none of the above are true.
		 * Don't call this method yourself. It is used by the ActionQueue.
		 */
		inline void scheduleNextAction(long when, short channel);

		/**
		 * Get the ActionQueue index for when the action in a give channel is scheduled to be performed.
		 * Don't call this method yourself. It is a helper function for ActionQueue's disrupt().
		 */
		unsigned short actionSchedule(short channel) { return presentActionScheduledComplete_[channel]; }

		// Helper function for planAction and scheduleNextAction(long when, short channel);
		void continueAction(long when, short channel);
		void scheduleNextAction(short channel);

		/** Ask currently running script for the next action in a channel.
		 */
		void nextScriptAction(short channel);

	protected:
		mutable ActionAndParameter presentAction_[CHANNEL_COUNT];
		mutable ActionAndParameter plannedAction_[CHANNEL_COUNT];

		mutable unsigned short presentActionScheduledComplete_[CHANNEL_COUNT];


		ActionFeed* feed_;

	protected:
	};


	inline void ActionComponent
	::perform(long when, short channel) {
		const Action* a = presentAction_[channel].action();
		Parameter& p = presentAction_[channel].parameter();
		a->perform(when, *this, p);
	}


	inline void ActionComponent
	::scheduleNextAction(long when, short channel) {
		if(!isActive() || isDead()) {
			presentAction_[ channel ].resetAction();
			return;
		}
		
		ActionAndParameter& aap = presentAction_[channel];
		if(aap.isDisrupted()) {
			aap.resetAction();
			aap.setDisrupted(false);
			return;
		}

		if(aap.hasAction() && aap.action()->isContinuing(*this, aap.parameter())) {
			continueAction(when, channel);
			return;
		}

		if(!plannedAction_[channel].hasAction()) {
			nextScriptAction(channel);
		}

		// No action planned?
		if(aap.hasAction() && !plannedAction_[channel].hasAction()) {
			// Is present action repeating (until another is planned)?
			//ActionAndParameter& aap = presentAction_[channel];
			if(aap.action()->isRepeating(when, *this, aap.parameter())) {
				// If, yeah - plan to do present action again
				plannedAction_[channel] = presentAction_[channel];
			}
			else {
				// Or not, present action is complete
				presentAction_[channel].resetAction();
			}
		}

		// Still no action planned? Then we are finished
		if(!plannedAction_[channel].hasAction()) {
			return;
		}

		// Put planned action into action queue
		scheduleNextAction(channel);
	}

}

#endif
