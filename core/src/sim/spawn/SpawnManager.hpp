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


#ifndef SpawnManager_hpp
#define SpawnManager_hpp

#include "sim/sim.hpp"
#include "sim/SimComponentManager.hpp"


namespace se_core {

	/**
	 * Base class for functionality component managers.
	 */
	class _SeCoreExport SpawnManager : public SimComponentManager {
	public:
		/**
		 * Construct SpawnManager.
		 */
		SpawnManager();


		/**
		 * Destructor.
		 */
		virtual ~SpawnManager();

		const char* name() { return "SpawnManager"; }
		

		void step(long when);

		void initGame();
		void cleanupGame();

		static SpawnManager& singleton();


		void addFactory(SimCompositeFactory* factory);
		SimCompositeFactory* factory(const char* name);
		Composite* create(const char* name);
		void scheduleForDestruction(Composite& thing);
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
		Composite* thingsScheduledForDestruction_[ MAX_THINGS_FOR_DESTRUCTION ];
	};

}

#endif
