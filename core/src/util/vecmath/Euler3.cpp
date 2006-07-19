#include "Euler3.hpp"
#include "Quat4.hpp"
#include "util/error/Log.hpp"
#include <cmath>
#include <cstdio>

namespace se_core {

	const char* Euler3
	::toLog() const {
		sprintf(log_msg(), "(%d, %d, %d)", yaw_, pitch_, roll_);
		return log_msg();
	}


	char* Euler3
	::toString(char* buffer) const {
		sprintf(buffer, "(%d, %d, %d)", yaw_, pitch_, roll_);
		return buffer;
	}



	void Euler3
	::set(const Quat4& q1) {
		float test = q1.x_*q1.y_ + q1.z_*q1.w_;
		if (test > 0.4999999) { // singularity at north pole
			yaw_ = BrayT::fromRad(2 * ::atan2(q1.x_, q1.w_));
			roll_ = BrayT::DEG90;
			pitch_ = 0;
			return;
		}

		if (test < -0.4999999) { // singularity at south pole
			yaw_ = BrayT::fromRad(-2 * atan2(q1.x_, q1.w_));
			roll_ = BrayT::DEG270;
			pitch_ = 0;
			return;
		}

		double sqx = q1.x_*q1.x_;
		double sqy = q1.y_*q1.y_;
		double sqz = q1.z_*q1.z_;

		yaw_ = BrayT::fromRad(-atan2(2*q1.y_*q1.w_-2*q1.x_*q1.z_ , 1 - 2*sqy - 2*sqz));
		roll_ = BrayT::fromRad(asin(2*test));
		pitch_ = BrayT::fromRad(atan2(2*q1.x_*q1.w_-2*q1.y_*q1.z_ , 1 - 2*sqx - 2*sqz));

		normalize();
	}



	void Euler3
	::interpolate(const Euler3& a1, scale_t alpha) {
		// Scale distance from 0 braybrookians, rather than the number itself
		yaw_ += BrayT::scale(alpha, BrayT::mask(a1.yaw_ - yaw_));
		pitch_ += BrayT::scale(alpha, BrayT::mask(a1.pitch_ - pitch_));
		roll_ += BrayT::scale(alpha, BrayT::mask(a1.roll_ - roll_));

		normalize();
	}

}
