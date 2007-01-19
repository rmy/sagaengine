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


#include "OdeAreaComponent.hpp"
#include "OdeManager.hpp"
#include "OdeThingComponent.hpp"

using namespace se_core;

namespace se_ode {

	OdeAreaComponent
	::OdeAreaComponent(SimComposite* owner)
		: SimNodeComponent(sct_ODE, owner) {
	}


	OdeAreaComponent
	::~OdeAreaComponent() {
	}


	void OdeAreaComponent
	::setActive(bool state) {
		if(state) {
			spaceId_ = OdeManager::singleton().createSpace();
			geomId_ = dCreatePlane(spaceId_, 0, 1, 0, 0);
		}
		else {
			dGeomDestroy(geomId_);
			OdeManager::singleton().destroySpace(spaceId_);
		}
	}


	void OdeAreaComponent
	::update() {
		MultiSimNodeComponent::TreeIterator it(children());
		while(it.hasNext()) {
			OdeThingComponent& otc = static_cast<OdeThingComponent&>(it.next());
			otc.update();
		}
	}


	void OdeAreaComponent
	::applyForces() {
		MultiSimNodeComponent::TreeIterator it(children());
		while(it.hasNext()) {
			OdeThingComponent& otc = static_cast<OdeThingComponent&>(it.next());
			otc.applyForces();
		}
	}


}
