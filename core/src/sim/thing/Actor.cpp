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


#include "Actor.hpp"
#include "../sim.hpp"
#include "../react/all.hpp"
#include "../action/all.hpp"
#include "../area/Area.hpp"
#include "../area/sim_area.hpp"
#include "../custom/Abilities.hpp"
#include "../error/sim_error.hpp"
#include "../physics/Physics.hpp"
#include "../schema/SimSchema.hpp"
#include "../script/Script.hpp"
#include "../script/ShowingCutscene.hpp"
#include "../script/Cutscene.hpp"
#include "../stat/ViewPoint.hpp"
#include "../stat/MultiSimObject.hpp"
#include "../stat/SimObjectList.hpp"
#include "../stat/SortedSimObjectList.hpp"
#include "../message/all.hpp"
#include "util/error/Log.hpp"
#include "util/math/Math.hpp"
#include "util/config/all.hpp"
#include "sim/config/sim_config.hpp"
#include <cstdio>
#include <cstring>


namespace se_core {
	Actor
	::Actor(const char* name)
			: Thing(got_ACTOR, name)
			  , isActive_(false)
			  , currentScript_(0), currentPhysics_(0)
			  , showingCutscene_(0)
			  , collide_(&tcDefault)
			  , target_(0)
			  , spawnCount_(0) {
		physics_[currentPhysics_] = 0;
		scriptStack_[currentScript_] = 0;
		for(int i = 0; i < CHANNEL_COUNT; ++i) {
			presentActionScheduledComplete_[i] = 0;
			actionStage_[i] = 0;
		}
	}


	Actor
	::~Actor() {
		clearScripts();
	}


	void Actor
	::cleanup() {
		clearScripts();
		Thing::cleanup();
	}


	bool Actor
	::isMover() const {
		// Actors that has physics can move
		return hasPhysics();
	}


	void Actor
	::scheduleNextAction(short channel) {
		// Actions still in action queue after
		// scheduleForDestruction may try to plan
		// further actions
		if(isDead_) return;
		Assert(plannedAction_[channel].hasAction());
		//presentAction_[channel].setAction(*plannedAction_[channel].action());
		//presentAction_[channel].swapParameters(plannedAction_[channel]);
		presentAction_[channel] = plannedAction_[channel];
		plannedAction_[channel].resetAction();
		actionStage_[ channel ] = 0;

		// Add to action queue
		const Action* a = presentAction_[channel].action();
		Parameter& p = presentAction_[channel].parameter();
		presentActionScheduledComplete_[channel]
			= SimSchema::actionQueue[channel].add(*this, a->duration(*this, p));
		a->prepare(*this, p);
	}


	void Actor
	::continueAction(long when, short channel) {
		// Add to action queue
		const Action* a = presentAction_[channel].action();
		Parameter& p = presentAction_[channel].parameter();
		presentActionScheduledComplete_[channel]
			= SimSchema::actionQueue[channel].add(*this, a->duration(*this, p));
		a->prepare(*this, p);
	}


	void Actor
	::planAction(short channel, const Action& action, const Parameter* parameter) const {
		plannedAction_[channel].setAction(action);
		if(parameter) {
			plannedAction_[channel].copyParameter(*parameter);
		}
		if(!presentAction_[channel].hasAction() && plannedAction_[channel].hasAction()) {
			const_cast<Actor&>(*this).scheduleNextAction(channel);
		}
	}


	void Actor
	::clearPlannedAction(short channel) const {
		plannedAction_[channel].resetAction();
	}


	void Actor
	::disrupt() {
		for(int i = 0; i < CHANNEL_COUNT; ++i) {
			disrupt(i);
		}
	}


	bool Actor
	::disrupt(short channel) {
		if(!presentAction_[ channel ].hasAction()) return true;
		// Actions are only removed from ActionQueue if it is in a future
		// initiative. If it is in the initiative presently performed, it
		// will not be touched...
		bool didDisrupt = SimSchema::actionQueue[ channel ].disrupt(*this);
		if(didDisrupt) {
			presentAction_[ channel ].resetAction();
		}
		return didDisrupt;
	}


	void Actor
	::scheduleForDestruction() {
		// Don't schedule for destruction twice
		if(isDead_) return;
		// Stop any scripts and release memory held by them
		clearScripts();
		// Clear planned actions
		for(int i = 0; i < CHANNEL_COUNT; ++i) {
			clearPlannedAction(i);
		}
		// Disrupt actions i progress
		disrupt();
		Thing::scheduleForDestruction();
	}


	void Actor
	::leaveCurrentArea() {
		/*
		for(int i = 0; i < CHANNEL_COUNT; ++i) {
			clearPlannedAction(i);
		}
		disrupt();
		setTerrainMode(TS_VOID);
		*/
		Thing::leaveCurrentArea();
	}


	bool Actor
	::changeArea() {
		if(Thing::changeArea()) {
			// If the area is active, the actor is now active
			setActive(nextPos().area()->isActive());
			// Any scripts run when this Actor enters the area??
			nextPos().area()->enter(*this);
			// Truly changing area
			return true;
		}
		return false;
	}


	Cutscene* Actor
	::findRunnableCutscene(Actor& actor) {
		SimObjectList::iterator_type it = actor.cutscenes().iterator();

		Cutscene* c = 0;
		while(it != SimObjectList::NULL_NODE) {
			c = SimSchema::simObjectList.nextCutscene(it);
			if(!questGoals().contains(c->afterTheseGoals())) {
				c = 0;
				continue;
			}
			if(questGoals().sharesAny(c->beforeTheseGoals())) {
				c = 0;
				continue;
			}
			break;
		}
		return c;
	}


	void Actor
	::say(const char* message) {
		SimSchema::messageCentral.speech(*this, message);
	}


	void Actor
	::sound(const char* snd) {
		SimSchema::soundCentral.sound(*this, snd);
	}


	void Actor
	::nextScriptAction(short channel) {
		Parameter& p = plannedAction_[channel].parameter();
		const Action* a = script()->nextAction(*this, channel, scriptData(), p);
		if(a) {
			plannedAction_[channel].setAction(*a);
			if(!presentAction_[channel].hasAction()) {
				scheduleNextAction(channel);
			}
		}
	}


	void Actor
	::setDefaultScript(const Script* s) {
		if(!isActive() || currentScript_ != 0) {
			if(scriptStack_[0]) {
				scriptStack_[0]->release(scriptData_[0]);
			}
			scriptStack_[0] = s;
			scriptData_[0] = scriptStack_[0]->init(*this);
			return;
		}

		if(script()) script()->release(scriptData_[currentScript_]);
		scriptStack_[0] = s;
		scriptData_[0] = scriptStack_[0]->init(*this);
		for(int i = 0; i < CHANNEL_COUNT; ++i) {
			nextScriptAction(i);
		}
	}


	void Actor
	::clearScripts() {
		if(showingCutscene_) {
			removeFromShowingCutscene();
		}
		while(currentScript_ > 0) {
			scriptStack_[ currentScript_ ]->release( scriptData_[ currentScript_ ] );
			scriptStack_[ currentScript_-- ] = 0;
		}
		if(scriptStack_[0] != 0) {
			scriptStack_[ 0 ]->release( scriptData_[ 0 ] );
			scriptStack_[ 0 ] = 0;
		}
	}


	void Actor
	::pushScript(const char* name) {
		// Asser(!isDead_);
		const Script* s = SimSchema::sortedSimObjectList().script(name);
		pushScript(s);
	}


	void Actor
	::pushScript(const Script* s) {
		for(int i = 0; i < CHANNEL_COUNT; ++i) {
			clearPlannedAction(i);
			disrupt(i);
		}

		// If there already is a script running...
		if(script() && !script()->isStacker()) {
			// Release script data
			popScript();
		}

		if(currentScript_ == 0 && scriptStack_[currentScript_] != 0) {
			++currentScript_;
		}

		Assert(currentScript_ < SCRIPT_STACK_SIZE);
		// Set the new script
		scriptStack_[currentScript_] = s;
		// Init script data
		scriptData_[currentScript_] = script()->init(*this);

		// If script is active
		if(isActive()) {
			// Plan actions for all channels
			for(short i = 0; i < CHANNEL_COUNT; ++i) {
				nextScriptAction(i);
			}
		}
	}


	void Actor
	::popScript() {
		// Asser(!isDead_);
		for(int i = 0; i < CHANNEL_COUNT; ++i) {
			clearPlannedAction(i);
		}
		disrupt();
		if(currentScript_ == 0) {
			Assert(script());
			script()->release(scriptData_[currentScript_]);
			scriptData_[currentScript_] = 0;
			scriptStack_[currentScript_] = 0;
			return;
		}

		if(script()) script()->release(scriptData_[currentScript_]);
		--currentScript_;
		if(!script()) return;

		script()->reinit(*this, scriptData());
		if(!isActive())
			return;
		for(short i = 0; i < CHANNEL_COUNT; ++i) {
			nextScriptAction(i);
		}
	}


	void Actor
	::stopScript() {
		if(showingCutscene_) {
			removeFromShowingCutscene();
		}
		else {
			Assert(script());
			script()->release(scriptData_[currentScript_]);
			scriptData_[currentScript_] = 0;
			currentScript_ = 0;
			scriptStack_[0] = 0;
			disrupt();
		}
	}


	void Actor
	::popPhysics() {
		Assert(currentPhysics_ > 0);
		--currentPhysics_;
	}


	void Actor
	::pushPhysics(const Physics* ph) {
		if(hasPhysics() && physics().isStacker()) {
			// Increase script counter
			++currentPhysics_;
		}
		Assert(currentPhysics_ < MAX_PHYSICS_STACK_SIZE - 1);
		physics_[ currentPhysics_ ] = ph;
	}


	void Actor
	::setShowingCutscene(ShowingCutscene* sc, const Script* script) {
		Assert(!showingCutscene_);
		showingCutscene_ = sc;
		showingCutscene_->addMember(*this);
		pushScript(script);
	}


	void Actor
	::setNoCutsceneShowing() {
		if(showingCutscene_) {
			popScript();
			showingCutscene_ = 0;
		}
	}


	void Actor
	::removeFromShowingCutscene() {
		if(showingCutscene_) {
			showingCutscene_->removeMember(*this);
		}
	}


	void Actor
	::stopShowingCutscene() {
		if(showingCutscene_) {
			showingCutscene_->freeMembers();
		}
	}


	void Actor
	::setActive(bool state) {
		if(state == isActive_) return;
		isActive_ = state;

		// Setting to active??
		if(isActive_) {
			// Start script
			if(script()) {
				// Init (or reinit) scripts data block
				script()->reinit(*this, scriptData());
				//scriptData_ = script()->init(*this);
				// Start script in all action channels
				for(int i = 0; i < CHANNEL_COUNT; ++i) {
					nextScriptAction(i);
				}
			}
		}
		// Setting to inactive
		else {
			// Clear action in all channels
			for(int i = 0; i < CHANNEL_COUNT; ++i) {
				clearPlannedAction(i);
			}
			// Disrupt actions in progress
			disrupt();

			// Reset movmement
			//nextPos().resetSpeed();
			//nextPos().setMovementMode(STAND);
		}
	}


	Thing* Actor
	::spawn(const char* thingName, int spawnPointId, long deniedTsMask) {
		// Get spawn point displace and face direction
		const SpawnPoint* sp = spawnPoint(spawnPointId);
		Assert(sp && "Spawn point does not exist");
		ViewPoint vp(*sp);

		// Calculate world coor of spawn point
		const PosNode* node = this;
		while(node != 0 && node != pos().area()) {
			vp.coor_.rotate(node->pos().face_);
			vp.coor_.add(node->pos().coor_);
			vp.face_.rotate(node->pos().face_);
			node = node->pos().parent();
		}

		// Spawn it in area (with area as parent)
		Thing* t = area()->spawn(thingName, vp, deniedTsMask);

		// Avoid collision with spawner
		if(t) t->setSpawner(this);

		return t;
	}

	/*
	Thing* Actor
	::spawn(const char* thingName) {
		// Create thing at same position and with same direction as spawner
		Thing* t = area()->spawn(thingName, pos());

		// Avoid collision with spawner
		t->setSpawner(this);

		return t;
	}

	
	Thing* Actor
	::spawn(const char* thingName, Vector3& disp) {
		// Create thing at same position and with same direction as spawner
		ViewPoint vp(pos());
		//p.setNoLayer();
		vp.coor_.add(disp);
		Thing* t = area()->spawn(thingName, vp);

		// Avoid collision with spawner
		t->setSpawner(this);

		return t;
	}
	*/

}
