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


#include "LevelDesignSnap.hpp"
#include "sim/thing/Actor.hpp"
#include <se_core.hpp>
#include <cmath>

using namespace se_core;

namespace se_editor {

	void LevelDesignSnap
	::perform(long when, ActionComponent& perf, Parameter& parameter) const {
		PosComponent::Ptr pPos(perf);
		Point3& c = pPos->nextPos().localCoor();
		c.x_ = floor(c.x_ * 2 + .5f) / 2.0f;
		c.y_ = floor(c.y_ * 2 + .5f) / 2.0f;
		c.z_ = floor(c.z_ * 2 + .5f) / 2.0f;

		Euler3& f = pPos->nextPos().localFace();
		f.yaw_ = BrayT::to8(f.yaw_);
		f.pitch_ = BrayT::to8(f.pitch_);
		f.roll_ = BrayT::to8(f.roll_);

		PhysicsComponent::Ptr pPhysics(perf);
		pPhysics->nextMove().work_.vp_.face_.yaw_ = f.yaw_;

		pPos->nextPos().updateWorldViewPoint();
	}


	short LevelDesignSnap
	::duration(ActionComponent& performer, Parameter& parameter) const {
		return 1;
	}


	const LevelDesignSnap actionLevelDesignSnap;
}
