#include "Tuple4.hpp"
#include "../math/CoorT.hpp"
#include <cstdio>

namespace se_core {

	bool Tuple4
	::epsilonEquals(const Tuple4& t1, coor_t epsilon) const {
		return (CoorT::abs(t1.x_ - x_) <= epsilon) &&
			(CoorT::abs(t1.y_ - y_) <= epsilon) &&
			(CoorT::abs(t1.z_ - z_) <= epsilon) &&
			(CoorT::abs(t1.w_ - w_) <= epsilon);
	}


	char* Tuple4
	::toString(char* buffer) const {
		sprintf(buffer, "(%f,%f,%f,%f)", x_ * 1.0, y_ * 1.0, z_ * 1.0, w_ * 1.0);
		return buffer;
	}


	const char* Tuple4
	::toLog() const {
		sprintf(log_msg(), "(%f, %f, %f, %f)", x_, y_, z_, w_);
		return log_msg();
	}

	bool Tuple4
	::isNan() const {
		return x_ != x_ || y_ != y_ || z_ != z_ || w_ != w_;
	}

}
