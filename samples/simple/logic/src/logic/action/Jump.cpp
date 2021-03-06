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
#include "util/vecmath/all.hpp"
#include "sim/physics/PhysicsComponent.hpp"
#include "sim/action/ActionComponent.hpp"

using namespace se_core;


namespace logic {

	short Jump
	::duration(se_core::ActionComponent& performer, se_core::Parameter& parameter) const {
		return 1;
	}


	bool Jump
	::isContinuing(se_core::ActionComponent &performer, se_core::Parameter& parameter) const {
		return false;
	}


	void Jump
	::perform(long when, ActionComponent& performer, se_core::Parameter& parameter) const {
		PhysicsComponent::Ptr pPhysics(performer.owner());
		PosComponent::Ptr pPos(performer.owner());

		// Can only jump if not in free air.
		if(!pPos->pos().isGrounded()) {
			// Set upwards speed
			pPhysics->nextMove().force_.y_ += COOR_RES;
			pPos->nextPos().setGrounded(false);
		}
	}


	const Jump actionJump;
}
