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
#include "../stat/sim_stat.hpp"
#include "../pos/Anim.hpp"
#include "../pos/Pos.hpp"
#include "util/vecmath/Point3.hpp"
#include "util/type/String.hpp"
#include "../script/ScriptComponent.hpp"
#include "../action/ActionComponent.hpp"
#include "../physics/PhysicsComponent.hpp"


namespace se_core {

	/**
	 * An actor is an in-game thing that may perform Action()s.
	 */
	class _SeCoreExport Actor : public Thing, public ActorData {
	public:
		//TODO:
		void stopScript() {
			scriptComponent_->stopScript();
		}

		void popScript() {
			scriptComponent_->popScript();
		}

		void pushScript(const Script* s) {
			scriptComponent_->pushScript(s);
		}

		void setDefaultScript(const Script* s) {
			scriptComponent_->setDefaultScript(s);
		}


		bool hasActiveScript() { 
			return scriptComponent_->hasActiveScript();
		}
		const Script* script() { 
			return scriptComponent_->script();
		}

		Cutscene* findRunnableCutscene(Actor& actor) { return 0; }
		
		void planAction(short channel, const Action& action, const Parameter* parameter = 0) const {
			actionComponent_->planAction(channel, action, parameter);
		}

		virtual void performDefaultMovementAction() const {
			WasHere();
		}

		void disrupt() {
			actionComponent_->disrupt();
		}

		// TODO
		void affect() {
			physicsComponent_->affect();
		}

		bool calcNextCoor() {
			return physicsComponent_->calcNextCoor();
		}

		bool isMover() const {
			return physicsComponent_->isMover();
		}

		Move& nextMove() {
			return physicsComponent_->nextMove();
		}

		const Move& move() const {
			return physicsComponent_->move();
		}

		void setDefaultPhysics(const Physics* ph) {
			physicsComponent_->setDefaultPhysics(ph);
		}

		void pushPhysics(const Physics* ph) {
			physicsComponent_->pushPhysics(ph);
		}

		void pushPhysics(const char* ph) {
			physicsComponent_->pushPhysics(ph);
		}

		void popPhysics() {
			physicsComponent_->popPhysics();
		}


	public:
		/** Constructor.
		 */
		Actor(const char* name);
		~Actor();

		void scheduleForDestruction();

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
		void sound(const char* snd);

		///////////////////////////////////////

		bool isPusher() const { return collide_ != 0; }

		void setShowingCutscene(ShowingCutscene* sc, const Script* script);
		ShowingCutscene* showingCutscene() { return showingCutscene_; }

		void removeFromShowingCutscene();
		void stopShowingCutscene();


		virtual void setActive(bool state);

		void setCollide(const ThingCollide* collide) { collide_ = collide; }
		inline bool pushThing(Thing& pushedThing) {
			return collide_->collide(*this, pushedThing);
		}

		const Actor* target() const { return target_; }
		Actor* target() { return target_; }
		bool hasTarget() const { return target_ != 0; }
		void setTarget(Actor* target) { target_ = target; }
		void resetTarget() { target_ = 0; }

		virtual bool isPlayer() { return false; }

		Thing* spawn(const char* name, int spawnPointId, long deniedTsMask = 0);
		void incSpawnCount() { ++spawnCount_; }
		void decSpawnCount() { --spawnCount_; }
		int spawnCount() { return spawnCount_; }

		MultiSimObject& cutscenes() { return cutscenes_; }
		MultiSimObject& cutsceneMemberships() { return cutsceneMemberships_; }
		MultiSimObject& questGoals() { return questGoals_; }

	protected:
		friend class SimEngine;
		friend class Area;

	protected:
		friend class ShowingCutscene;

		/** ShowingCutscene uses this to free members. */
		void setNoCutsceneShowing();

	protected:
		// Group shorts for compiler
		ShowingCutscene* showingCutscene_;

		MultiSimObject cutscenes_;
		MultiSimObject cutsceneMemberships_;
		MultiSimObject questGoals_;

		const ThingCollide* collide_;
		Actor* target_;

		int spawnCount_;

		ScriptComponent* scriptComponent_;
		ActionComponent* actionComponent_;
		PhysicsComponent* physicsComponent_;
	};



}

#endif
