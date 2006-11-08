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


#include "ThingManager.hpp"
#include "ThingFactory.hpp"
#include "Thing.hpp"
#include "../SimCompositeFactory.hpp"
#include "../script/Cutscene.hpp"
#include "../stat/MultiSimObject.hpp"
#include "util/error/Log.hpp"
#include <cstring>
#include <cstdio>


namespace se_core {

	ThingManager
	::~ThingManager() {
		reset();
		while(factoryCount_ > 0) {
			delete factories_[ --factoryCount_ ];
		}
		factoryCount_ = 0;
	}


	void ThingManager
	::addFactory(SimCompositeFactory* factory) {
		Assert(factoryCount_ < MAX_FACTORIES - 1);
		factories_[ factoryCount_++ ] = factory;
	}


	SimCompositeFactory* ThingManager
	::factory(const char* name) {
		for(int i = 0; i < factoryCount_; ++i) {
			if(strcmp(factories_[i]->name(), name) == 0) {
				return factories_[i];
			}
		}
		LogFatal((sprintf(log_msg(), "ThingManager.cpp: Tried to get thing with unkown name: %s - %d", name, factoryCount_), log_msg()));
		return 0;
	}


	void ThingManager
	::addCutscene(const char* name, Cutscene* cutscene) {
		/*
		for(int i = 0; i < factoryCount_; ++i) {
			if(strcmp(factories_[i]->name(), name) == 0) {
				factories_[i]->multiSimObject(MGO_CUTSCENES).add(*cutscene);
				return;
			}
		}
		LogFatal((sprintf(log_msg(), "Tried to add cs '%s' to unkown thing: %s - %d", cutscene->name(), name, factoryCount_), log_msg()));
		*/
	}


	void ThingManager
	::addCutsceneMembership(const char* name, Cutscene* cutscene) {
		LogFatal("Not implemented");
		/*
		for(int i = 0; i < factoryCount_; ++i) {
			if(strcmp(factories_[i]->name(), name) == 0) {
				factories_[i]->multiSimObject(MGO_CUTSCENE_MEMBERSHIPS).add(*cutscene);
				return;
			}
		}
		LogFatal((sprintf(log_msg(), "ThingManager.cpp: Tried to add cutscene '%s' to thing with unkown name: %s - %d", cutscene->name(), name, factoryCount_), log_msg()));
		*/
	}


	Thing* ThingManager
	::create(const char* name) {
		for(int i = 0; i < factoryCount_; ++i) {
			if(strcmp(factories_[i]->name(), name) == 0) {
				Thing* t =  static_cast<Thing*>(factories_[i]->create());
				t->factory_ = factories_[i];
				return t;
			}
		}
		LogFatal((sprintf(log_msg(), "ThingManager.cpp: Tried to create thing with unkown name: %s - %d", name, factoryCount_), log_msg()));
		return 0;
	}


	void ThingManager
	::scheduleForDestruction(Thing& thing) {
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


	void ThingManager
	::neutraliseDestructions() {
		for(int i = 0; i < nextDestructionCount_; ++i) {
			thingsScheduledForDestruction_[ i ]->cleanup();
		}
	}


	void ThingManager
	::performDestructions() {
		while(destructionCount_ > 0) {
			//LogMsg("Dest count: " << destructionCount_);
			Thing* t = thingsScheduledForDestruction_[ --destructionCount_ ];
			Assert(t->factory_);
			// Let the facory that created the object do the deletion.
			t->factory_->release(t);
			thingsScheduledForDestruction_[ destructionCount_ ] =
				thingsScheduledForDestruction_[ --nextDestructionCount_ ];
		}
		neutraliseDestructions();
		destructionCount_ = nextDestructionCount_;
	}


	void ThingManager
	::reset() {
		performDestructions();
		performDestructions();
	}

}
