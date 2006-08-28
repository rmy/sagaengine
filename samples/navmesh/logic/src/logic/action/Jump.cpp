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


#include "LogicPre.hpp"
#include "Jump.hpp"

using namespace se_core;


namespace logic {

	short Jump
	::duration(se_core::Actor& performer, se_core::Parameter& parameter) const {
		return 1;
	}


	bool Jump
	::isContinuing(se_core::Actor &performer, se_core::Parameter& parameter) const {
		return false;
	}


	void Jump
	::perform(long when, Actor& performer, se_core::Parameter& parameter) const {
		// Can only jump if not in free air.
		if(performer.pos().isGrounded()) {
			// Set upwards speed
			performer.nextMove().force_.y_ += COOR_RES;
			performer.nextPos().setGrounded(false);
		}
	}


	const Jump actionJump;
}

