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
#include "comp/schema/CompSchema.hpp"
#include "comp/list/NodeComponentList.hpp"
#include "util/error/Log.hpp"


using namespace se_core;

namespace se_basic {

	NavMeshAreaComponent
	::NavMeshAreaComponent(Composite* owner, const se_core::ComponentFactory* factory) 
			: RootChildComponent(se_core::sct_NAVMESH, owner, factory) {
	}


	NavMeshAreaComponent
	::~NavMeshAreaComponent() {
	}


	void NavMeshAreaComponent
	::setActive(bool state) {
		if(state) {
			NodeComponent* c = static_cast<NodeComponent*>(CompSchema::activeRoot().component(type_));
			if(c) {
				setParent(*c);
			}
		}
		else {
			resetParent();
		}
	}

}

