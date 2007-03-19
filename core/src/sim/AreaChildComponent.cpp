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


#include "SimComponent.hpp"
#include "AreaChildComponent.hpp"
#include "SimObject.hpp"
#include "comp/Composite.hpp"
#include "schema/SimSchema.hpp"
#include "util/error/Log.hpp"


namespace se_core {

	AreaChildComponent
	::AreaChildComponent(enum SimComponentType type, Composite* owner)
		: SimNodeComponent(type, owner) {
	}


	AreaChildComponent
	::~AreaChildComponent() {
	}


	void AreaChildComponent
	::zoneChanged(int zoneType, Composite* newArea, Composite* oldArea) {
		if(zoneType != st_AREA)
			return;

		if(newArea) {
			SimNodeComponent* n = static_cast<SimNodeComponent*>(newArea->component(type()));
			setParent(*n);
		}
		else {
			resetParent();
		}
	}

}
