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
#include "util/vecmath/ViewPoint.hpp"
#include "../stat/all.hpp"
#include "../area/Area.hpp"

namespace se_core {

	Move
	::Move() : velocity_(0, 0, 0), mass_(SCALE_RES)
			, linearFriction_(.5f * SCALE_STEP), angularFriction_(SCALE_STEP)
			, didBounce_(false), bounceMaintain_(.75f), affectCode_(0) {
		angularVelocity_.setIdentity();
		torque_.setIdentity();
		force_.reset();
		work_.speed_ = 0;
		work_.vp_.setIdentity();
	}


	void Move
	::setMove(const Move& original) {
		force_ = original.force_;
		velocity_ = original.velocity_;
	}


	void Move
	::changeSpeed(coor_t speed) {
		work_.speed_ = speed;
		velocity_.setForward(work_.speed_, work_.vp_.face_.yaw_, 0);
	}


	void Move
	::changeYaw(bray_t yaw) {
		work_.vp_.face_.yaw_ = yaw;
		velocity_.setForward(work_.speed_, work_.vp_.face_.yaw_, 0);
	}


	void Move
	::changeMovement(bray_t yaw, coor_t speed) {
		work_.vp_.face_.yaw_ = yaw;
		work_.speed_ = speed;
		velocity_.setForward(work_.speed_, work_.vp_.face_.yaw_, 0);
	}


	void Move
	::resetForces() {
		// Forces must be reapplied every step
		force_.reset();
		torque_.reset();
		didBounce_ = false;
	}


}

#endif // SE_OWN_MOVE
