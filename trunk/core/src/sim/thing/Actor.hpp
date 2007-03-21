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

#include "comp/Composite.hpp"
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
	class _SeCoreExport Actor : public Component {
	public:
		typedef Ptr<Actor, sct_BLOB> Ptr;

		/** Constructor.
		 */
		Actor(Composite* owner);
		~Actor();


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

		void affect() {
			if(!isDead())
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

		void scheduleForDestruction() {
			owner()->scheduleForDestruction();
		}

		coor_t walkSpeed() const {
			return statComponent_->walkSpeed();
		}

		Abilities* abilities() {
			return statComponent_->abilities(); 
		}

		/**
		 * Have the Actor say something.
		 * The message is looked up in the language file and typically shown
		 * in a speech bubble.
		 *
		 * @param message The message name as occurring in a speech bubble.
		 */
		void say(const char* message);
		void sound(const char* snd);


		const Composite* target() const { return statComponent_->target(); }
		Composite* target() { return statComponent_->target(); }
		bool hasTarget() const { return statComponent_->hasTarget(); }
		void setTarget(Composite* target) { statComponent_->setTarget(target); }
		void resetTarget() { statComponent_->resetTarget(); }

		const Pos& pos() const { return posComponent_->pos(); }
		Pos& nextPos() const { return posComponent_->nextPos(); }

		bool hasDefaultAction() const {
			return statComponent_->hasDefaultAction();
		}

		const Action* defaultAction(Parameter& out) const { 
			return statComponent_->defaultAction(out);
		}

		/**
		 *
		 */
		void planDefaultAction() const;


	protected:
		PosComponent* posComponent_;
		SpawnComponent* spawnComponent_;
		ScriptComponent* scriptComponent_;
		ActionComponent* actionComponent_;
		PhysicsComponent* physicsComponent_;
		StatComponent* statComponent_;
	};

}

#endif
