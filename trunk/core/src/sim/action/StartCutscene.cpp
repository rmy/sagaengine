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
#include "../sim.hpp"
#include "../config/all.hpp"
#include "../schema/SimSchema.hpp"
#include "../script/Script.hpp"
#include "../script/Cutscene.hpp"
#include "../script/QuestGoal.hpp"
#include "../stat/SortedSimObjectList.hpp"
#include "../stat/MultiSimObject.hpp"
#include "../stat/SimObjectIterator.hpp"
#include "../thing/Actor.hpp"
#include "../../util/error/Log.hpp"
#include <cstdio>



namespace se_core {


	void StartCutscene
	::perform(long when, Actor& performer, Parameter& parameter) const {
		if(performer.target()->type() == got_THING) return;
		Actor* a = const_cast<Actor*>(performer.target());

		Cutscene* c = performer.findRunnableCutscene(*a);
		if(!c) return;
		if(c->questGoal()) {
			performer.questGoals().add(*(c->questGoal()));
		}

		if(c) c->setScripts(*performer.area(), performer, a);
	}


	const StartCutscene actionStartCutscene;
}
