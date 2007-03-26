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


#ifndef SortedSimObjectList_hpp
#define SortedSimObjectList_hpp

#include "../sim.hpp"
#include "../action/sim_action.hpp"
#include "../config/sim_config.hpp"
#include "../react/sim_react.hpp"
#include "../physics/sim_physics.hpp"
#include "../script/sim_script.hpp"
#include "../signal/sim_signal.hpp"

namespace se_core {
	class _SeCoreExport SortedSimObjectList {
	public:
		SortedSimObjectList();
		virtual ~SortedSimObjectList();
		void add(SimObject* go);
		void remove(SimObject* go);

		bool has(SimObjectType type, const char* name);

		//const Script* script(const char* name);
		//const Physics* physics(const char* name);
		//const ThingCollide* collide(const char* name);
		//const Action* action(const char* name);
		//const Signal* signal(const char* name);
		//Cutscene* cutscene(const char* name);
		//QuestGoal* questGoal(const char* name);


	private:
		unsigned int find(SimObjectType type, const char* name);
		SimObject* get(SimObjectType type, const char* name);

		static const int MAX_GAME_OBJECTS = se_core::MAX_GAME_OBJECTS;
		SimObject** simObjects_;
		unsigned int simObjectCount_;
	};

}

#endif
