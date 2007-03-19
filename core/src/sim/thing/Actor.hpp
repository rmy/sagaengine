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

#include "../SimComposite.hpp"
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
#include "../pos/PosComponent.hpp"
#include "util/vecmath/Point3.hpp"
#include "util/type/String.hpp"
#include "../script/ScriptComponent.hpp"
#include "../spawn/SpawnComponent.hpp"
#include "../action/ActionComponent.hpp"
#include "../action/ActionAndParameter.hpp"
#include "../physics/PhysicsComponent.hpp"
#include "../react/CollisionComponent.hpp"
#include "../custom/StatComponent.hpp"
#include "../stat/MultiSimObject.hpp"


namespace se_core {

	/**
	 * An actor is an in-game thing that may perform Action()s.
	 */
	class _SeCoreExport Actor : public SimComposite {
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

		void planAction(short channel, const Action& action, const Parameter* parameter = 0) const {
			actionComponent_->planAction(channel, action, parameter);
		}

		void disrupt() {
			actionComponent_->disrupt();
		}

		// TODO
		void affect() {
			if(!isDead_)
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


		// TODO
		coor_t walkSpeed() const {
			return statComponent_->walkSpeed();
		}

		Abilities* abilities() {
			return statComponent_->abilities(); 
		}
		

	public:
		/** Constructor.
		 */
		Actor(const CompositeFactory* f);
		~Actor();

		/**
		 * Ask if this class will safely cast to a spcific
		 * SimObject subclass.
		 */
		bool isType(enum SimObjectType type) const {
			if(type == got_ACTOR) return true;
			//return Composite::isType(type);
			return false;
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


		const Actor* target() const { return static_cast<const Actor*>(target_.object()); }
		Actor* target() { return static_cast<Actor*>(target_.object()); }
		bool hasTarget() const { return !target_.isNull(); }
		void setTarget(Actor* target) { target_.set(target->ref()); }
		void resetTarget() { target_.reset(); }
		const Pos& pos() const { return posComponent_->pos(); }
		Pos& nextPos() const { return posComponent_->nextPos(); }

		virtual bool isPlayer() { return false; }

		/*
		SimComposite* spawn(const char* name, int spawnPointId, long deniedTsMask = 0);
		void incSpawnCount() { spawnComponent_->incSpawnCount(); }
		void decSpawnCount() { spawnComponent_->decSpawnCount(); }
		int spawnCount() { return spawnComponent_->spawnCount(); }
		*/

		bool hasDefaultAction() const {
			return defaultAction_.hasAction();
		}
		const ActionAndParameter& defaultAction() const { 
			return defaultAction_; 
		}
		const Action* defaultAction(Parameter& out) const { 
			out = defaultAction_.parameter();
			return defaultAction_.action(); 
		}
		void setDefaultAction(const Action& action, const Parameter* parameter = 0) { 
			defaultAction_.setAction(action);
			if(parameter) {
				defaultAction_.copyParameter(*parameter);
			}
		}
		void resetDefaultAction() { defaultAction_.resetAction(); }

		/**
		 *
		 */
		void planDefaultAction() const;


	protected:
		friend class SimEngine;
		friend class Area;

	protected:
		mutable ActionAndParameter defaultAction_;

		RefPtr target_;

		PosComponent* posComponent_;
		SpawnComponent* spawnComponent_;
		ScriptComponent* scriptComponent_;
		ActionComponent* actionComponent_;
		PhysicsComponent* physicsComponent_;
		StatComponent* statComponent_;
	};



}

#endif
