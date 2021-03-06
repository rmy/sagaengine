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


#include "SignalManager.hpp"
#include "SignalAreaComponent.hpp"
#include "../schema/SimSchema.hpp"
#include "../sim.hpp"
#include "../config/sim_config.hpp"


namespace se_core {
	SignalManager
	::SignalManager()
		: RootComponent(sct_SIGNAL) {
	}

	SignalManager
	::~SignalManager() {
	}


	SignalManager&  SignalManager
	::singleton() {
		static SignalManager instance;
		return instance;
	}


	void SignalManager
	::step(long when) {
		NodeComponentList::Iterator it(children_);
		while(it.hasNext()) {
			SignalAreaComponent& c = static_cast<SignalAreaComponent&>(it.next());
			c.propagate();
		}
	}


	void SignalManager
	::initGame() {
	}


	void SignalManager
	::cleanupGame() {
	}

}
