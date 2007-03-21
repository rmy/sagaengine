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


#include "RootChildComponent.hpp"
#include "comp/Composite.hpp"
#include "comp/schema/CompSchema.hpp"
#include "util/error/Log.hpp"


namespace se_core {

	RootChildComponent
	::RootChildComponent(int type, Composite* owner, const ComponentFactory* factory)
		: NodeComponent(type, owner, factory) {
	}


	RootChildComponent
	::~RootChildComponent() {
	}


	void RootChildComponent
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
