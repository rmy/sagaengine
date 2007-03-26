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


#ifndef SimObjectIterator_hpp
#define SimObjectIterator_hpp

#include "sim_stat.hpp"
#include "../sim.hpp"
#include "../thing/sim_thing.hpp"
#include "../script/sim_script.hpp"
#include "SimObjectList.hpp"

namespace se_core {

	class _SeCoreExport SimObjectIterator {
	public:
		SimObjectIterator();
		SimObjectIterator(MultiSimObject& mgo);
		void init(MultiSimObject& mgo);
		void init(short firstMode);
		bool hasNext();
		SimObject& next();
		//Actor& nextActor();
		//Cutscene& nextCutscene();
		//QuestGoal& nextQuestGoal();

	private:
		SimObjectList::iterator_type it_;
	};

}


#endif
