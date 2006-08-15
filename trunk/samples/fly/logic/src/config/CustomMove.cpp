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


#include "CustomMove.hpp"
#include "util/type/util_type.hpp"
#include "util/math/Trig.hpp"
#include "sim/stat/all.hpp"
#include "sim/area/Area.hpp"

namespace se_core {

	Move
	::Move() : velocity_(0, 0, 0) {
		angularVelocity_.setIdentity();
		flick();
		mass_ = 1.0f;
	}

	void Move
	::setMove(const Move& original) {
		force_ = original.force_;
		velocity_ = original.velocity_;
	}


	Vector3& Move
	::pushForce(Vector3& dest) const {
		dest.set(velocity_.x_, 0, velocity_.z_);
		return dest;
	}


	void Move
	::flick() {
		// Forces must be reapplied every step
		force_.reset();
		torque_.setIdentity();
	}


}
