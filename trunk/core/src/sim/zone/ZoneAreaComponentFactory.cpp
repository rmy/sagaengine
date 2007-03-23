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


#include "ZoneAreaComponentFactory.hpp"
#include "ZoneAreaComponent.hpp"
#include "comp/schema/CompSchema.hpp"
#include "comp/list/NodeComponentList.hpp"
#include "sim/schema/SimSchema.hpp"
#include "util/error/Log.hpp"
#include "Exit.hpp"



namespace se_core {

	ZoneAreaComponentFactory
	::ZoneAreaComponentFactory()
		: ComponentFactory(sct_ZONE) {
	}


	Component* ZoneAreaComponentFactory
	::create(Composite* owner) const {
		// Try to get existing component 
		// - allows overrides of default values
		// (Useful when loading saved games).
		ZoneAreaComponent::Ptr pZone(*owner);
		if(pZone == 0) {
			// Create new component
			pZone = new ZoneAreaComponent(owner, this);
			pZone->setExits(exits_, exitCount_);
		}

		return pZone;
	}

	void ZoneAreaComponentFactory
	::setExits(Exit* exits, int count) {
		AssertFatal(exits_ != 0, "Exits already defined for area.");

		exits_ = new Exit[ count ];
		for(int i = 0; i < count; ++i) {
			exits_[i] = exits[i];
		}
		exitCount_ = count;
	}

}
