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
#include "../physics/Physics.hpp"
#include "../schema/SimSchema.hpp"
#include "../script/Script.hpp"
#include "../script/ShowingCutscene.hpp"
#include "../script/Cutscene.hpp"
#include "util/vecmath/ViewPoint.hpp"
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
			, showingCutscene_(0)
			, collide_(&tcDefault)
			, spawnCount_(0) {
		actionComponent_ = new ActionComponent(this);
		scriptComponent_ = new ScriptComponent(this, actionComponent_);
		physicsComponent_ = new PhysicsComponent(this);
		statComponent_ = new StatComponent(this);
	}


	Actor
	::~Actor() {
		scriptComponent_->clearScripts();
		delete statComponent_;
		delete physicsComponent_;
		delete scriptComponent_;
		delete actionComponent_;
	}


	void Actor
	::cleanup() {
		actionComponent_->cleanup();
		scriptComponent_->clearScripts();
		Thing::cleanup();
	}


	void Actor
	::scheduleForDestruction() {
		// Don't schedule for destruction twice
		if(isDead_) return;
		// Stop any scripts and release memory held by them
		actionComponent_->cleanup();
		scriptComponent_->clearScripts();
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


	/*
	Cutscene* Actor
	::findRunnableCutscene(Actor& actor) {
		SimObjectList::iterator_type it = actor.cutscenes().iterator();

		Cutscene* c = 0;
		while(it != SimObjectList::end()) {
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
	*/


	void Actor
	::say(const char* message) {
		SimSchema::messageCentral.speech(*this, message);
	}


	void Actor
	::sound(const char* snd) {
		SimSchema::soundCentral.sound(*this, snd);
	}


	void Actor
	::stopShowingCutscene() {
		if(showingCutscene_) {
			showingCutscene_->freeMembers();
		}
	}

	void Actor
	::setNoCutsceneShowing() {
		if(showingCutscene_) {
			popScript();
			showingCutscene_ = 0;
		}
	}


	/*
	void Actor
	::setShowingCutscene(ShowingCutscene* sc, const Script* script) {
		Assert(!showingCutscene_);
		showingCutscene_ = sc;
		showingCutscene_->addMember(*this);
		pushScript(script);
	}


	void Actor
	::removeFromShowingCutscene() {
		if(showingCutscene_) {
			showingCutscene_->removeMember(*this);
		}
	}


	*/


	/*
	void Actor
	::setActive(bool state) {
		if(state == isActive_) return;
		isActive_ = state;

		scriptComponent_->setActive(state);
		actionComponent_->setActive(state);
	}
	*/


	Thing* Actor
	::spawn(const char* thingName, int spawnPointId, long deniedTsMask) {
		// Get spawn point displace and face direction
		const ViewPoint* sp = spawnPoint(spawnPointId);
		Assert(sp && "Spawn point does not exist");
		ViewPoint vp(*sp);

		// Calculate area coor of spawn point
		const PosNode* node = this;
		vp.add(pos().world_);
		vp.sub(position_.area()->pos().world_);

		// Spawn it in area (with area as parent)
		Thing* t = position_.area()->spawn(thingName, vp, deniedTsMask);

		// Avoid collision with spawner
		if(t) t->setSpawner(this);

		return t;
	}


	void Actor
	::planDefaultAction() const {
		if(!defaultAction_.hasAction()) return;
		actionComponent_->planAction(CHANNEL_EXTRA, defaultAction_);
	}

}
