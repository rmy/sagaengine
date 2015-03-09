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


#include "PulseaudioManager.hpp"
#include "PulseaudioAreaComponent.hpp"
#include "PulseaudioComponent.hpp"
#include "sim/sim.hpp"
#include "comp/comp.hpp"
#include "comp/schema/CompSchema.hpp"
#include "comp/list/NodeComponentList.hpp"
#include "sim/schema/SimSchema.hpp"
#include "util/error/Log.hpp"

using namespace se_pulseaudio;

namespace se_pulseaudio {
	PulseaudioManager
	::PulseaudioManager()
		: RootComponent(se_core::sct_PULSEAUDIO) {
	}

	PulseaudioManager
	::~PulseaudioManager() {
	}


	PulseaudioManager&  PulseaudioManager
	::singleton() {
		static PulseaudioManager instance;
		return instance;
	}


	void PulseaudioManager
	::step(long when) {
		/*
		MultiSimNodeComponent::Iterator it(children_);
		while(it.hasNext()) {
			PulseaudioAreaComponent& c = static_cast<PulseaudioAreaComponent&>(it.next());
			//c.propagate();
		}
		*/
	}


	void PulseaudioManager
	::initGame() {
	}


	void PulseaudioManager
	::cleanupGame() {
	}

}
