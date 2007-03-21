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


#include "%sManager.hpp"
#include "%sAreaComponent.hpp"
#include "%sComponent.hpp"
#include "sim/sim.hpp"
#include "sim/schema/SimSchema.hpp"
#include "sim/stat/MultiSimNodeComponent.hpp"
#include "util/error/Log.hpp"

using namespace se_core;

namespace %n {

	%sAreaComponent
	::%sAreaComponent(Composite* owner, const se_core::ComponentFactory* factory) 
			: AreaComponent(se_core::sct_%S, owner, factory) {
	}


	%sAreaComponent
	::~%sAreaComponent() {
	}


	void %sAreaComponent
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

