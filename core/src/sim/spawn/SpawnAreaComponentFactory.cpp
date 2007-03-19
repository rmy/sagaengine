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


#include "SpawnAreaComponentFactory.hpp"
#include "SpawnAreaComponent.hpp"
#include "sim/schema/SimSchema.hpp"
#include "sim/stat/MultiSimNodeComponent.hpp"
#include "util/error/Log.hpp"




namespace se_core {

	SpawnAreaComponentFactory
	::SpawnAreaComponentFactory()
		: SimComponentFactory(sct_SPAWN) {
	}


	Component* SpawnAreaComponentFactory
	::create(Composite* owner) const {
		// Try to get existing component 
		// - allows overrides of default values
		// (Useful when loading saved games).
		SpawnAreaComponent::Ptr pSpawn(*owner);
		if(pSpawn == 0) {
			// Create new component
			pSpawn = new SpawnAreaComponent(owner, this);
		}

		return pSpawn;
	}

}
