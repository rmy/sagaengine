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


#include "StartCutscene.hpp"
#include "sim/sim.hpp"
#include "sim/config/all.hpp"
#include "sim/schema/SimSchema.hpp"
#include "sim/script/Script.hpp"
#include "sim/script/Cutscene.hpp"
#include "sim/script/QuestGoal.hpp"
#include "sim/stat/SortedSimObjectList.hpp"
#include "sim/stat/MultiSimObject.hpp"
#include "sim/stat/SimObjectIterator.hpp"
#include "sim/thing/Actor.hpp"
#include "util/error/Log.hpp"
#include <cstdio>


using namespace se_core;

namespace se_basic {


	void StartCutscene
	::perform(long when, ActionComponent& perf, Parameter& parameter) const {
		LogFatal("TODO");

		// TODO:
		/*
		Actor& performer = *perf.toActor();
		PosComponent::Ptr pPos(perf);
		if(performer.target()->type() == got_THING) return;
		Actor* a = const_cast<Actor*>(performer.target());

		Cutscene* c = performer.findRunnableCutscene(*a);
		if(!c) return;
		if(c->questGoal()) {
			performer.questGoals().add(*(c->questGoal()));
		}

		if(c) c->setScripts(*performer.nextPos().area()->toArea(), performer, a);
		*/
	}


	const StartCutscene actionStartCutscene;
}
