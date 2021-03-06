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
#include "Sprint.hpp"
#include "util/vecmath/Vector3.hpp"
#include "../schema/LogicSchema.hpp"
#include "sim/action/ActionComponent.hpp"
#include "sim/pos/PosComponent.hpp"
#include "sim/physics/PhysicsComponent.hpp"

using namespace se_core;


namespace logic {

	void Sprint
	::perform(long when, ActionComponent& performer, se_core::Parameter& parameter) const {
		Vector3 f(0, 0, -0.75f * COOR_RES);
		PhysicsComponent* pPhysics = PhysicsComponent::get(performer);
		PosComponent* pPos = PosComponent::get(performer);
		f.rotate(pPos->pos().localFace());

		pPhysics->nextMove().clearForces();
		pPhysics->nextMove().addForce(f);
	}


	const Sprint actionSprint;
}

