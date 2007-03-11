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
#include "../spawn/SpawnComponent.hpp"
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
			: SimComposite(got_ACTOR, name)
			, showingCutscene_(0) {
		posComponent_ = new PosComponent(this);
		spawnComponent_ = new SpawnComponent(this, posComponent_);
		actionComponent_ = new ActionComponent(this);
		scriptComponent_ = new ScriptComponent(this, actionComponent_);
		physicsComponent_ = new PhysicsComponent(this, posComponent_);
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


	void Actor
	::planDefaultAction() const {
		if(!defaultAction_.hasAction()) return;
		actionComponent_->planAction(CHANNEL_EXTRA, defaultAction_);
	}

}
