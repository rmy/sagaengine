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


#ifndef engine_thing_Actor_hpp
#define engine_thing_Actor_hpp

#include "Thing.hpp"
#include "../custom/ActorData.hpp"
#include "../action/sim_action.hpp"
#include "../custom/Move.hpp"
#include "../physics/Physics.hpp"
#include "../action/Action.hpp"
#include "../action/ActionAndParameter.hpp"
#include "../react/sim_react.hpp"
#include "../react/ThingCollide.hpp"
#include "../script/sim_script.hpp"
#include "../script/Script.hpp"
#include "../stat/sim_stat.hpp"
#include "../stat/Anim.hpp"
#include "../stat/Coor.hpp"
#include "../stat/Pos.hpp"
#include "util/type/String.hpp"


namespace se_core {

	/**
	 * An actor is an in-game thing that may perform Action()s.
	 */
	class Actor : public Thing, public ActorData {
	public:
		/** Constructor.
		 */
		Actor(const char* name);
		~Actor();

		/** Reset any scripts and do Thing::cleanup().
		 */
		void cleanup();

		/**
		 * Ask if this class will safely cast to a spcific
		 * SimObject subclass.
		 */
		bool isType(enum SimObjectType type) const {
			if(type == got_ACTOR) return true;
			return Thing::isType(type);
		}

		/**
		 * Returns true if the Actor is inside an active area.
		 */
		inline bool isActive() const { return isActive_; }

		/**
		 * Plan a new action.
		 * Plan an Action that will be scheduled when the present one is
		 * finished. Will overwrite any action that is presently planned
		 * but not yet in the ActionQueue.
		 */
		void planAction(short channel, const Action& action, const Parameter* parameter = 0) const;

		/**
		 * Clear planned action.
		 * Clear an action that is waiting for being entered into the
		 * ActionQueue, but hasn't yet gotten there (because another
		 * Action for the same Actor and channel is already in the queue).
		 */
		void clearPlannedAction(short channel) const;

		/**
		 * Disrupt all actions the this Actor has in the ActionQueue.
		 * Actions that are performed in the on-going initiative are not
		 * disrupted.
		 */
		void disrupt();

		/**
		 * Disrupt an action the this Actor has in a given ActionQueue channel.
		 * Actions that are performed in the on-going initiative are not
		 * disrupted.
		 */
		bool disrupt(short channel);

		/**
		 * Plan to perform the default movement action.
		 *
		 * Plans the default movement action when the action that is presently
		 * in the ActionQueue is completed, if no other action is planned
		 * for that channel. The default movement Action is typically Walk
		 * or Stand. This method allows the player character to resume movement
		 * after the player has pushed a buttton to do something else.
		 *
		 * If you want to force planning this action, do a
		 * clearPlannedAction(...) first.
		 *
		 * @see clearPlannedAction
		 */
		virtual void planDefaultMovementAction() const {}

		//ActionParameter& param(short channel) const { return actionParameters_[channel]; }

		/////////////////////////////////////////////////////////////////

		/**
		 * Schedule this Actor for destruction.
		 * It will be destroyed when the on-going initative is performed,
		 * allowing the actor to complete any action it is performing this
		 * initiative.
		 */
		void scheduleForDestruction();

		/**
		 * Enter a new Area.
		 * And warn the Area that a new visitor has arrived.
		 */
		bool changeArea();

		/**
		 * Leave the current Area.
		 * And warn the Area that someone has left.
		 */
		void leaveCurrentArea();

		/**
		 * Have the Actor say something.
		 * The message is looked up in the language file and typically shown
		 * in a speech bubble.
		 *
		 * @param message The message name as occurring in a speech bubble.
		 */
		void say(const char* message);

		///////////////////////////////////////

		/** Push a script on the script stack of the Actor.
		 *
		 * The script will be executed until it is popped, or
		 * another script is pushed on top of it.
		 */
		void pushScript(const Script* s);

		/** Push a script with a given name on the script stack of the Actor.
		 *
		 * The script will be executed until it is popped, or
		 * another script is pushed on top of it.
		 */
		void pushScript(const char* name);

		/** Pop a script from the script stack of the Actor.
		 *
		 * The script below the present script on the script stack will be resumed.
		 */
		void popScript();

		/** Return true if the Actor has an active script.
		 */
		inline bool hasActiveScript() const {
			return scriptStack_[currentScript_] != 0;
		}

		/**
		 * Get the active script of the Actor.
		 */
		inline const Script* script() const {
			return scriptStack_[currentScript_];
		}

		inline ScriptData* scriptData() {
			return scriptData_[currentScript_];
		}


		/** Clear all scripts from the script stack.
		 *
		 * Remove Actor from any running Cutscene, and stop all
		 * script execution. Performet at cleanup.
		 */
		void clearScripts();

		/** Ask currently running script for the next action in a channel.
		 *
		 *
		 */
		void nextScriptAction(short channel);

		/**
		 * Set the default script of the Actor.
		 * Sets the script that the Actor will run when no cutscenes are showing.
		 * Actions perfomed inside a Cutscene (and outside if you want) that wants to
		 * set the script that the Actor uses after the Cutscene is done, should use
		 * this method.
		 */
		void setDefaultScript(const Script* s);

		/**
		 * Stop the presently active script.
		 * Removes the actor from the showing cutscene, if any.
		 * If not, stop the default script and set the Actor to have no default
		 * script.
		 */
		void stopScript();

		Cutscene* findRunnableCutscene(Actor& actor);

		///////////////////////////////////////

		bool isMover() const;
		bool isPusher() const { return collide_ != 0; }

		void setShowingCutscene(ShowingCutscene* sc, const Script* script);
		ShowingCutscene* showingCutscene() { return showingCutscene_; }

		void removeFromShowingCutscene();
		void stopShowingCutscene();


		inline unsigned short actionStage(short channel) { return actionStage_[channel]; }
		void incrActionStage(short channel) { ++actionStage_[channel]; }
		void setActionStage(short channel, short s) { ++actionStage_[channel] = s; }
		virtual void setActive(bool state);

		bool calcNextCoor() {
			move_ = nextMove_;
			nextMove_.flick();
			physics().calcNext(*this, pos(), nextPos(), nextAnim(), nextMove());

			// Any change in render position?
			didMove_ = !nextPos().viewPointEquals(position_);

			// Colliding with area geometry?
			// (TODO: Doing this test here optimises away one collision loop
			// but it should probably be moved into the physics object
			// for greater flexibility, and even more speed.)
			if(didMove_ && physics().isBlocked(*this, pos(), nextPos())) {
				// No movement this step
				resetFutureCoor();
				return false;
			}
			return didMove_;
		}


	    void affect() {
			physics().affect(*this);
		}

		void pushPhysics(const Physics *ph);
		void popPhysics();

		inline const Move& move() const {
			return move_;
		}

		inline Move& nextMove() {
			return nextMove_;
		}

		void setCollide(const ThingCollide* collide) { collide_ = collide; }
		inline bool pushThing(Thing& pushedThing) {
			return collide_->collide(*this, pushedThing);
		}

		const Actor* target() const { return target_; }
		Actor* target() { return target_; }
		bool hasTarget() const { return target_ != 0; }
		void setTarget(Actor* target) { target_ = target; }

		virtual bool isPlayer() { return false; }

		//Thing* spawn(const char* name);
		Thing* spawn(const char* name, int spawnPointId, long deniedTsMask = 0);
		//Thing* spawn(const char* name, Vector3& disp);
		void incSpawnCount() { ++spawnCount_; }
		void decSpawnCount() { --spawnCount_; }
		int spawnCount() { return spawnCount_; }

		void setDefaultPhysics(const Physics* ph) {
			physics_[0] = ph;
		}

		MultiSimObject& cutscenes() { return cutscenes_; }
		MultiSimObject& cutsceneMemberships() { return cutsceneMemberships_; }
		MultiSimObject& questGoals() { return questGoals_; }

	protected:
		friend class ActionQueue;

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



	protected:
		friend class SimEngine;
		friend class Area;

		/**
		 * Reset the next coordinate.
		 * Sets the next coordinate to be the same as the present one.
		 */
		inline void resetFutureCoor() { didMove_ = false; nextPos().setPos(position_); }

		bool isBlockedByTerrain() {
			return physics().isBlocked(*this, pos(), nextPos());
		}

	protected:
		friend class ThingFactory;
		friend class ThingDefinition;

		inline bool hasPhysics() const {
			return (physics_[0] != 0);
		}
		inline const Physics& physics() {
			return *physics_[currentPhysics_];
		}
		inline const Physics& physics() const {
			return *physics_[currentPhysics_];
		}

	protected:
		friend class ShowingCutscene;

		/** ShowingCutscene uses this to free members. */
		void setNoCutsceneShowing();

	protected:
		/**
		 * Signifies wether this Actor is active at the moment.
		 * An active actor is able to plan and perform actions.
		 */
		bool isActive_;

		mutable ActionAndParameter presentAction_[CHANNEL_COUNT];
		mutable ActionAndParameter plannedAction_[CHANNEL_COUNT];

		mutable unsigned short presentActionScheduledComplete_[CHANNEL_COUNT];
		mutable unsigned short actionStage_[CHANNEL_COUNT];

		// Group shorts for compiler
		short currentScript_;
		short currentPhysics_;

		static const short SCRIPT_STACK_SIZE = 6;
		const Script* scriptStack_[SCRIPT_STACK_SIZE];
		ScriptData* scriptData_[SCRIPT_STACK_SIZE];

		ShowingCutscene* showingCutscene_;

		MultiSimObject cutscenes_;
		MultiSimObject cutsceneMemberships_;
		MultiSimObject questGoals_;

		static const short MAX_PHYSICS_STACK_SIZE = 3;
		const Physics* physics_[ MAX_PHYSICS_STACK_SIZE ];

		Move move_, nextMove_;

		const ThingCollide* collide_;
		Actor* target_;

		int spawnCount_;

	};


	inline void Actor
	::perform(long when, short channel) {
		const Action* a = presentAction_[channel].action();
		Parameter& p = presentAction_[channel].parameter();
		a->perform(when, *this, p);
	}


	inline void Actor
	::scheduleNextAction(long when, short channel) {
		if(!isActive_ || isDead_) {
			presentAction_[ channel ].resetAction();
			return;
		}

		ActionAndParameter& aap = presentAction_[channel];
		if(aap.hasAction() && aap.action()->isContinuing(*this, aap.parameter())) {
			continueAction(when, channel);
			return;
		}

		if(script() && !plannedAction_[channel].hasAction()) {
			// TODO:
			nextScriptAction(channel);
			//ActionAndParameter& aap = plannedAction_[channel];
			//const Action* a = script()->nextAction(*this, channel, scriptData_, aap.parameter);
			//aap.setAction(*a);
			//plannedAction_[channel].setAction(script()->nextAction(*this, channel, scriptData_));
		}

		// No action planned?
		if(!plannedAction_[channel].hasAction()) {
			// Is present action repeating (until another is planned)?
			ActionAndParameter& aap = presentAction_[channel];
			if(aap.action()->isRepeating(when, *this, aap.parameter())) {
				// If, yeah - plan to do present action again
				//plannedAction_[channel].setAction(*presentAction_[channel].action());
				//plannedAction_[channel].swapParameters(presentAction_[channel]);
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
