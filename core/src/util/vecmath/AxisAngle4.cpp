/*
   Copyright (C) 1997,1998,1999
   Kenji Hiranabe, Eiwa System Management, Inc.

   This program is free software.
   Implemented by Kenji Hiranabe(hiranabe@esm.co.jp),
   conforming to the Java(TM) 3D API specification by Sun Microsystems.

   Permission to use, copy, modify, distribute and sell this software
   and its documentation for any purpose is hereby granted without fee,
   provided that the above copyright notice appear in all copies and
   that both that copyright notice and this permission notice appear
   in supporting documentation. Kenji Hiranabe and Eiwa System Management,Inc.
   makes no representations about the suitability of this software for any
   purpose.  It is provided "AS IS" with NO WARRANTY.

   Copyright (C) 2006
   Rune Myrland, Skalden Studio AS
   SagaEngine adaptions, involving conversion from floating point values to fixed point values.
   Kenji Hiranabe's license apply.
*/

#include "util/error/Log.hpp"
#include "../math/CoorT.hpp"
#include "../math/Trig.hpp"
#include "AxisAngle4.hpp"
#include "Quat4.hpp"
#include "Euler3.hpp"
#include <cmath>

namespace se_core {

	void AxisAngle4
	::set(const Quat4& q1) {
		setFromQuat(q1.x_, q1.y_, q1.z_, q1.w_);
	}

	void AxisAngle4
	::setFromQuat(coor_t x, coor_t y, coor_t z, coor_t w) {
		// This logic can calculate angle without normalization.
		// The direction of (x,y,z) and the sign of rotation cancel
		// each other to calculate a right answer.

		// |sin a/2|, w = cos a/2
		coor_t sin_a2 = CoorT::sqrt(CoorT::pow2(x) + CoorT::pow2(y) + CoorT::pow2(z));  

		// 0 <= angle <= PI , because 0 < sin_a2
		angle_ = BrayT::fromRad(2.0*::atan2(sin_a2, w)); 
		x_ = x;
		y_ = y;
		z_ = z;
	}


	void AxisAngle4
	::set(const coor_t len, const bray_t yaw) {
		x_ = Trig::sinScale(len, yaw);
		z_ = Trig::cosScale(len, yaw);
	}


	void AxisAngle4
	::set(const coor_t len, const bray_t yaw, const bray_t pitch) {
		set(len, yaw);

		y_ = Trig::sinScale(len, pitch);
		x_ = Trig::cosScale(x_, pitch);
		z_ = Trig::cosScale(z_, pitch);
	}


	void AxisAngle4
	::set(const coor_t len, const Euler3& a1) {
		set(len, a1.yaw_, a1.pitch_);
		angle_ = a1.roll_;
	}


	const char* AxisAngle4
	::toLog() const {
		sprintf(log_msg(), "AxisAngle4(%f, %f, %f, %d)", x_ * 1.0, y_ * 1.0, z_ * 1.0, angle_);
		return log_msg();
	}
}

