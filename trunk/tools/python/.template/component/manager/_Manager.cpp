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
#include "comp/comp.hpp"
#include "comp/schema/CompSchema.hpp"
#include "comp/list/NodeComponentList.hpp"
#include "sim/schema/SimSchema.hpp"
#include "util/error/Log.hpp"

using namespace %n;

namespace %n {
	%sManager
	::%sManager()
		: RootComponent(se_core::sct_%S) {
	}

	%sManager
	::~%sManager() {
	}


	%sManager&  %sManager
	::singleton() {
		static %sManager instance;
		return instance;
	}


	void %sManager
	::step(long when) {
		/*
		MultiSimNodeComponent::Iterator it(children_);
		while(it.hasNext()) {
			%sAreaComponent& c = static_cast<%sAreaComponent&>(it.next());
			//c.propagate();
		}
		*/
	}


	void %sManager
	::initGame() {
	}


	void %sManager
	::cleanupGame() {
	}

}
