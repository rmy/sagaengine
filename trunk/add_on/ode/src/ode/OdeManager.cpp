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


#include "OdePre.hpp"
#include "OdeManager.hpp"

namespace se_ode {

	OdeManager
	::OdeManager() {
	}


	OdeManager
	::~OdeManager() {
	}


	OdeManager&  OdeManager
	::singleton() {
		static OdeManager instance;
		return instance;
	}


	void OdeManager
	::step(long when) {
		// Flip coors for next physics step
		//flip(when);

		// Precalc coordinate for the next frame
		//performPhysics(when + TIMESTEP_INTERVAL);

		// Test and resolve collisions between game things and between
		// game things and walls or other terrain obstructions.
		//testCollisions(when, when + TIMESTEP_INTERVAL);

		// Affect thing with effects of movements
		//affect(when + TIMESTEP_INTERVAL);
	}

}
