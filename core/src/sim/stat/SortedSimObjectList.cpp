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


#include "SortedSimObjectList.hpp"
#include "../SimObject.hpp"
#include "../action/Action.hpp"
#include "../physics/Physics.hpp"
#include "../react/ThingCollide.hpp"
#include "../script/Script.hpp"
#include "../script/Cutscene.hpp"
#include "../script/QuestGoal.hpp"
#include "../../util/error/Log.hpp"
#include <cstring>
#include <cstdio>

namespace se_core {

	SortedSimObjectList
	::SortedSimObjectList()
		: simObjects_(new SimObject*[ MAX_GAME_OBJECTS ]), simObjectCount_(0) {
		LogMsg("Creating SortedSimObjectList");
	}


	SortedSimObjectList
	::~SortedSimObjectList() {
		delete[] simObjects_;
		LogMsg("Destroying SortedSimObjectList");
	}


	void SortedSimObjectList
	::add(SimObject* go) {
		Assert(simObjectCount_ < MAX_GAME_OBJECTS - 1);
		Assert(go != 0);

		unsigned int index = find(go->type(), go->name());
		// Don't add to the list if it is there already...
		if(index == simObjectCount_ || (simObjects_[ index ]->type() != go->type() || strcmp(simObjects_[ index ]->name(), go->name()) != 0)) {
			for(unsigned int i = simObjectCount_; i > index; --i) {
				simObjects_[ i ] = simObjects_[ i - 1];
			}
			simObjects_[ index ] = go;
			++simObjectCount_;
		}
		LogMsg("Registered SimObject '" << go->name() << "' of type " << go->type());
	}


	void SortedSimObjectList
	::remove(SimObject* go) {
		unsigned int index = find(go->type(), go->name());
		if(simObjects_[ index ]->type() != go->type() || strcmp(simObjects_[ index ]->name(), go->name()) != 0) {
			return;
		}
		--simObjectCount_;
		for(unsigned int i = index; i < simObjectCount_; ++i) {
			simObjects_[ i ] = simObjects_[ i + 1];
		}
	}


	const Action* SortedSimObjectList
	::action(const char* name) {
		return static_cast<Action*>(get(got_ACTION, name));
	}


	Cutscene* SortedSimObjectList
	::cutscene(const char* name) {
		//LogMsg("Cutscene");
		return static_cast<Cutscene*>(get(got_CUTSCENE, name));
	}


	const Script* SortedSimObjectList
	::script(const char* name) {
		//LogMsg("Script");
		//LogMsg(name);
		return static_cast<Script*>(get(got_SCRIPT, name));
	}


	const Physics* SortedSimObjectList
	::physics(const char* name) {
		//LogMsg("Physics");
		return static_cast<Physics*>(get(got_PHYSICS, name));
	}


	const ThingCollide* SortedSimObjectList
	::collide(const char* name) {
		//LogMsg("Collide");
		return static_cast<ThingCollide*>(get(got_THING_COLLIDE, name));
	}


	QuestGoal* SortedSimObjectList
	::questGoal(const char* name) {
		//LogMsg("QuestGoal");
		return static_cast<QuestGoal*>(get(got_QUEST_GOAL, name));
	}


	unsigned int SortedSimObjectList
	::find(SimObjectType type, const char* name) {
		unsigned int min = 0;
		unsigned int max = simObjectCount_;
		unsigned int middle;
		while(max > min) {
			middle = (max + min) >> 1;
			if(simObjects_[ middle ]->type() < type
					|| (simObjects_[ middle ]->type() == type && strcmp(simObjects_[middle]->name(), name) < 0)) {
				min = middle + 1;
			}
			else {
				max = middle;
			}
		}
		return min;
	}


	SimObject* SortedSimObjectList
	::get(SimObjectType type, const char* name) {
		unsigned int index = find(type, name);
		Assert(index >= 0 && index <= simObjectCount_);

		DebugExec(if(!(simObjects_[ index ]->type() == type && strcmp(simObjects_[ index ]->name(), name) == 0))) {
			LogMsg(index << ": " << type << ", " << name);
			LogMsg(simObjects_[ index ]->type() << ", " << simObjects_[ index ]->name());
			LogFatal("Not found: " << name);
		}

		return simObjects_[ index ];
	}


	bool SortedSimObjectList
	::has(SimObjectType type, const char* name) {
		unsigned int index = find(type, name);
		if(index == simObjectCount_ || simObjects_[ index ]->type() != type || strcmp(simObjects_[ index ]->name(), name) != 0) {
			return false;
		}
		return true;
	}

}
