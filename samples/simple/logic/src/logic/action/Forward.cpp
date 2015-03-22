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
#include "Forward.hpp"
#include "util/vecmath/all.hpp"
#include "sim/physics/PhysicsComponent.hpp"
#include "sim/action/ActionComponent.hpp"

using namespace se_core;


namespace logic {

	void Forward
	::perform(long when, ActionComponent& performer, Parameter& parameter) const {
		Param* p = static_cast<Param*>(parameter.data(sizeof(Param)));
		PhysicsComponent::Ptr pPhysics(performer.owner());
		PosComponent::Ptr pPos(performer.owner());

		Vector3 force(0, 0, 0.05f * p->speed_);
		force.rotate(pPos->pos().localFace());
		pPhysics->nextMove().addForce(force);
	}


	void Forward
	::param(float speed, Parameter& out) const {
		Param* p = static_cast<Param*>(out.data(sizeof(Param)));
		p->speed_ = speed;
	}



	const Forward actionForward;
}
