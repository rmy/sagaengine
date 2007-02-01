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


#ifndef ThingManager_hpp
#define ThingManager_hpp

#include "sim_thing.hpp"
#include "../sim.hpp"
#include "../config/sim_config.hpp"
#include "../script/sim_script.hpp"

namespace se_core {

	class _SeCoreExport ThingManager {
	public:
		ThingManager() : factoryCount_(0), destructionCount_(0)
					   , nextDestructionCount_(0) {}
		virtual ~ThingManager();
		void addFactory(SimCompositeFactory* factory);
		void addCutscene(const char* name, Cutscene* cutscene);
		void addCutsceneMembership(const char* name, Cutscene* cutscene);
		SimCompositeFactory* factory(const char* name);
		SimComposite* create(const char* name);
		void scheduleForDestruction(SimComposite& thing);
		void neutraliseDestructions();
		void performDestructions();
		void reset();
		void resetAll();

	private:
		int factoryCount_;
		int destructionCount_;
		int nextDestructionCount_;
		static const int MAX_FACTORIES = MAX_GAME_TYPES;
		SimCompositeFactory* factories_[ MAX_FACTORIES ];
		static const int MAX_THINGS_FOR_DESTRUCTION = MAX_GAME_OBJECTS;
		SimComposite* thingsScheduledForDestruction_[ MAX_THINGS_FOR_DESTRUCTION ];
	};

}

#endif
