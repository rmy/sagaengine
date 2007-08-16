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


#include "SpawnComponentFactory.hpp"
#include "SpawnComponent.hpp"
#include "../sim.hpp"
#include "../SimEngine.hpp"
#include "../config/all.hpp"
#include "../stat/all.hpp"
#include "util/vecmath/ViewPoint.hpp"


namespace se_core {

	SpawnComponentFactory
	::SpawnComponentFactory()
		: ComponentFactory(sct_STAT), spawnPointCount_(0), spawnPoints_(0) {
	}


	Component* SpawnComponentFactory
	::create(Composite* owner) const {
		// Try to get existing component 
		// - allows overrides of default values
		// (Useful when loading saved games).
		SpawnComponent::Ptr c(*owner);
		if(c == 0) {
			// Create new component
			c = new SpawnComponent(owner, this);
		}
		c->setSpawnPoints(spawnPointCount_, spawnPoints_);
		return c;
	}


	void SpawnComponentFactory
	::setSpawnPoints(int count, ViewPoint* const* spawnPoints) {
		if(count == 0 && spawnPoints_ != 0)
			return;
		spawnPoints_ = new ViewPoint*[ count ];

		spawnPointCount_ = count;
		for(int i = 0; i < spawnPointCount_; ++i) {
			spawnPoints_[ i ] = spawnPoints[ i ];
		}
	}


}
