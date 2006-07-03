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
		changeMovement(0, 0);
	}

	void Move
	::setMove(const Move& original) {
		force_ = original.force_;
		velocity_ = original.velocity_;
	}


	void Move
	::changeSpeed(coor_t speed) {
		speed_ = speed;
		velocity_.setForward(speed_, yaw_);
	}


	void Move
	::changeYaw(bray_t yaw) {
		yaw_ = yaw;
		velocity_.setForward(speed_, yaw_);
	}


	void Move
	::updateVelocity(const Quat4& q) {
		//yaw_ = yaw;
		//velocity_.setForward(speed_, yaw_);
		velocity_.set(0, 0, speed_);
		velocity_.rotate(q);
	}


	void Move
	::changeMovement(bray_t yaw, coor_t speed) {
		yaw_ = yaw;
		speed_ = speed;
		velocity_.setForward(speed_, yaw_);
	}


	void Move
	::changeMovement(Quat4& face, coor_t speed) {
		velocity_.set(0, 0, speed);
		velocity_.rotate(face);
	}


	Force& Move
	::pushForce(Force& dest) const {
		dest.set(velocity_.x_, 0, velocity_.z_);
		return dest;
	}


	void Move
	::flick() {
		// Forces must be reapplied every step
		force_.reset();
		torque_.reset();
	}


	void Move
	::calcNext(const Pos& original, Coor& dest) {
		dest.x_ = original.x_ + velocity_.x_ + force_.x_;
		dest.z_ = original.z_ + velocity_.z_ + force_.z_;
		if(original.layer() >= 0 && original.hasArea())
			dest.y_ = original.area()->groundHeight(dest);
		else
			dest.y_ = original.y_ + velocity_.y_ + force_.y_;

		// TODO:
		//dest.face_.add(original.face_, angularVelocity_);
	}


	void Move
	::fastInterpolate(const Pos& original, Coor& dest) {
		dest.x_ = original.x_ + velocity_.x_ + force_.x_;
		dest.y_ = original.y_ + velocity_.y_ + force_.y_;
		dest.z_ = original.z_ + velocity_.z_ + force_.z_;
	}


	void Move
	::fastInterpolate(const Pos& original, const Force& force, Coor& dest) {
		dest.x_ = original.x_ + velocity_.x_ + force.x_;
		dest.y_ = original.y_ + velocity_.y_ + force.y_;
		dest.z_ = original.z_ + velocity_.z_ + force.z_;
	}


	void Move
	::fastFutureInterpolate(const Pos& original, Coor& dest) {
		dest.x_ = original.x_ + velocity_.x_ + force_.x_;
		dest.y_ = original.y_ + velocity_.y_ + force_.y_;
		dest.z_ = original.z_ + velocity_.z_ + force_.z_;
	}


	void Move
	::fastFutureInterpolate(const Pos& original, const Force& force, Coor& dest) {
		dest.x_ = original.x_ + velocity_.x_ + force_.x_ + force.x_;
		dest.y_ = original.y_ + velocity_.y_ + force_.y_ + force.y_;
		dest.z_ = original.z_ + velocity_.z_ + force_.z_ + force.z_;
	}

}

#endif // SE_OWN_MOVE
