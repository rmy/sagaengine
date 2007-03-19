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
#include "AreaComponent.hpp"
#include "SimObject.hpp"
#include "SimComposite.hpp"
#include "schema/SimSchema.hpp"
#include "util/error/Log.hpp"


namespace se_core {

	AreaComponent
	::AreaComponent(enum SimComponentType type, SimComposite* owner, const SimComponentFactory* factory)
		: SimNodeComponent(type, owner, factory) {
	}


	AreaComponent
	::~AreaComponent() {
	}


	void AreaComponent
	::setActive(bool state) {
		if(state) {
			SimNodeComponent* c = static_cast<SimNodeComponent*>(SimSchema::activeRoot().component(type_));
			if(c) {
				setParent(*c);
			}
		}
		else {
			resetParent();
		}
	}

}