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


#include "SpawnManager.hpp"
#include "SpawnComponent.hpp"
#include "sim/schema/SimSchema.hpp"
#include "comp/list/NodeComponentList.hpp"
#include "util/error/Log.hpp"
#include "comp/CompositeFactory.hpp"
#include "comp/Composite.hpp"
#include <cstring>

using namespace se_core;

namespace se_core {
	SpawnManager
	::SpawnManager()
			: RootComponent(sct_SPAWN)
			, factoryCount_(0), destructionCount_(0)
			, nextDestructionCount_(0) {
		factories_ = new CompositeFactory*[ MAX_FACTORIES ];
		thingsScheduledForDestruction_ = new Composite*[ MAX_THINGS_FOR_DESTRUCTION ];
	}

	SpawnManager
	::~SpawnManager() {
		delete [] factories_;
		delete [] thingsScheduledForDestruction_;
		resetAll();
	}


	SpawnManager&  SpawnManager
	::singleton() {
		static SpawnManager instance;
		return instance;
	}


	void SpawnManager
	::step(long when) {
		/*
		NodeComponentList::Iterator it(children_);
		while(it.hasNext()) {
			SpawnAreaComponent& c = static_cast<SpawnAreaComponent&>(it.next());
			//c.propagate();
		}
		*/
	}


	void SpawnManager
	::initGame() {
	}


	void SpawnManager
	::cleanupGame() {
	}



	void SpawnManager
	::addFactory(CompositeFactory* factory) {
		Assert(factoryCount_ < MAX_FACTORIES - 1);
		LogDetail(factoryCount_ << ": " << factory->name());
		factories_[ factoryCount_++ ] = factory;
	}


	CompositeFactory* SpawnManager
	::factory(const char* name) {
		for(int i = 0; i < factoryCount_; ++i) {
			if(strcmp(factories_[i]->name(), name) == 0) {
				return factories_[i];
			}
		}
		LogFatal("SpawnManager.cpp: Tried to get thing with unkown name: " << name << " - " - factoryCount_);
		return 0;
	}



	Composite* SpawnManager
	::create(const char* name) {
		for(int i = 0; i < factoryCount_; ++i) {
			if(strcmp(factories_[i]->name(), name) == 0) {
				const CompositeFactory* f = factories_[i];
				Composite* t = static_cast<Composite*>(f->create());
				//t->setFactory(f);
				return t;
			}
		}
		LogFatal("Tried to create thing with unkown name: " << name);
		return 0;
	}


	void SpawnManager
	::scheduleForDestruction(Composite& thing) {
		Assert(nextDestructionCount_ < MAX_THINGS_FOR_DESTRUCTION || "Too many things scheduled for destruction already.");

		// TODO: The below is debug code, remove for performance increase when stable
		DebugExec(for(int i = 0; i < nextDestructionCount_; ++i)) {
			DebugExec(if(&thing == thingsScheduledForDestruction_[ i ])) {
				DbgLogFatal("Trying to destroy thing twice.");
			}
		}
		// End of debug code
		thingsScheduledForDestruction_[ nextDestructionCount_++ ] = &thing;
	}


	void SpawnManager
	::neutraliseDestructions() {
		for(int i = 0; i < nextDestructionCount_; ++i) {
			thingsScheduledForDestruction_[ i ]->cleanup();
		}
	}


	void SpawnManager
	::performDestructions() {
		while(destructionCount_ > 0) {
			//LogDetail("Dest count: " << destructionCount_);
			Composite* t = thingsScheduledForDestruction_[ --destructionCount_ ];
			Assert(t->factory());
			// Let the facory that created the object do the deletion.
			t->factory()->release(t);
			thingsScheduledForDestruction_[ destructionCount_ ] =
				thingsScheduledForDestruction_[ --nextDestructionCount_ ];
		}
		neutraliseDestructions();
		destructionCount_ = nextDestructionCount_;
	}


	void SpawnManager
	::reset() {
		neutraliseDestructions();
		performDestructions();
		performDestructions();
	}

	void SpawnManager
	::resetAll() {
		reset();
		while(factoryCount_ > 0) {
			delete factories_[ --factoryCount_ ];
		}
		destructionCount_ = 0;
		nextDestructionCount_ = 0;
		factoryCount_ = 0;
	}


}
