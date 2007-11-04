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


#include "ScriptComponentFactory.hpp"
#include "ScriptComponent.hpp"
#include "../sim.hpp"
#include "../SimEngine.hpp"
#include "../config/all.hpp"
#include "../script/all.hpp"


namespace se_core {

	ScriptComponentFactory
	::ScriptComponentFactory()
		: ComponentFactory(sct_SCRIPT), scriptCount_(0) {
	}


	Component* ScriptComponentFactory
	::create(Composite* owner) const {
		// Try to get existing component 
		// - allows overrides of default values
		// (Useful when loading saved games).
		ScriptComponent::Ptr c(*owner);
		if(c == 0) {
			// Create new component
			c = new ScriptComponent(owner, this);
		}

		for(int i = 0; i < scriptCount_; ++i) {
			c->pushScript(scripts_[i]);
		}

		return c;
	}

	void ScriptComponentFactory
	::pushScript(const char* name) {
		Assert(scriptCount_ < MAX_SCRIPTS);
		scripts_[ scriptCount_++ ] = Script::lookup(name);
	}

}
