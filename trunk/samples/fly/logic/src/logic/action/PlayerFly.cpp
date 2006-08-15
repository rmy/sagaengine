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
#include "PlayerFly.hpp"
#include "../schema/LogicSchema.hpp"

using namespace se_core;


namespace logic {

	void PlayerFly
	::perform(long when, Actor& performer, se_core::Parameter& parameter) const {
		Param* p = static_cast<Param*>(parameter.data(sizeof(Param)));
		performer.nextMove().torque_.set(p->torque_);

		Vector3 f(0, 0, -p->speed_);
		f.rotate(performer.pos().localFace());
		performer.nextMove().addForce(f);
	}


	void PlayerFly
	::param(float dirLR, float dirUD, float dirRoll, float speed, Parameter& out) const {
		Param* p = static_cast<Param*>(out.data(sizeof(Param)));
		float r = ::sqrt(speed);
		p->torque_.set(
					BrayT::fromDeg(dirLR * .5f  / r)
					, BrayT::fromDeg(-dirUD * .5f / r)
					, BrayT::fromDeg(dirRoll * 1 / r)
					);
		p->speed_ = 0.15f * speed;
	}


	const PlayerFly actionPlayerFly;
}

