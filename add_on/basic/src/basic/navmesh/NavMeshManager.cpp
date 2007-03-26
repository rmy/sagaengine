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


#include "NavMeshManager.hpp"
#include "NavMeshAreaComponent.hpp"
#include "NavMeshComponent.hpp"
#include "sim/sim.hpp"
#include "comp/comp.hpp"
#include "comp/schema/CompSchema.hpp"
#include "comp/list/NodeComponentList.hpp"
#include "sim/schema/SimSchema.hpp"
#include "util/error/Log.hpp"

using namespace se_basic;

namespace se_basic {
	NavMeshManager
	::NavMeshManager()
		: RootComponent(se_core::sct_NAVMESH) {
	}

	NavMeshManager
	::~NavMeshManager() {
	}


	NavMeshManager&  NavMeshManager
	::singleton() {
		static NavMeshManager instance;
		return instance;
	}


	void NavMeshManager
	::step(long when) {
		/*
		MultiSimNodeComponent::Iterator it(children_);
		while(it.hasNext()) {
			NavMeshAreaComponent& c = static_cast<NavMeshAreaComponent&>(it.next());
			//c.propagate();
		}
		*/
	}


	void NavMeshManager
	::initGame() {
	}


	void NavMeshManager
	::cleanupGame() {
	}

}
