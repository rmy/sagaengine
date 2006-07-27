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


#include "Move.hpp"

#ifndef SE_OWN_MOVE

#include "util/type/util_type.hpp"
#include "util/math/Trig.hpp"
#include "../stat/all.hpp"
#include "../area/Area.hpp"

namespace se_core {

	Move
	::Move() : velocity_(0, 0, 0) {
		angularVelocity_.setIdentity();
		torque_.setIdentity();
		force_.reset();
	}

	void Move
	::setMove(const Move& original) {
		force_ = original.force_;
		velocity_ = original.velocity_;
	}


	void Move
	::changeSpeed(coor_t speed) {
		speed_ = speed;
		velocity_.setForward(speed_, yaw_, 0);
	}


	void Move
	::changeYaw(bray_t yaw) {
		yaw_ = yaw;
		velocity_.setForward(speed_, yaw_, 0);
	}


	void Move
	::changeMovement(bray_t yaw, coor_t speed) {
		yaw_ = yaw;
		speed_ = speed;
		velocity_.setForward(speed_, yaw_, 0);
	}


	void Move
	::flick() {
		// Forces must be reapplied every step
		force_.reset();
		torque_.reset();
	}


}

#endif // SE_OWN_MOVE
