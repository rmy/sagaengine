#include "Tuple3.hpp"
#include "../math/CoorT.hpp"
#include "../math/Math.hpp"
#include <cstdio>

namespace se_core {

	bool Tuple3
	::epsilonEquals(const Tuple3& t1, coor_t epsilon) const {
		return (CoorT::abs(t1.x_ - x_) <= epsilon) &&
			(CoorT::abs(t1.y_ - y_) <= epsilon) &&
			(CoorT::abs(t1.z_ - z_) <= epsilon);
	}


	char* Tuple3
	::toString(char* buffer) const {
#		ifdef SE_FIXED_POINT
		sprintf(buffer, "(%d,%d,%d)", x_, y_,z_);
#		else
		sprintf(buffer, "(%f,%f,%f)", x_, y_,z_);
#		endif
		return buffer;
	}


	void Tuple3
	::scale(scale_t s, const Tuple3& t1) {
		x_ = CoorT::scale(s, t1.x_);
		y_ = CoorT::scale(s, t1.y_);
		z_ = CoorT::scale(s, t1.z_);
	}


	void Tuple3
	::scale(scale_t s) {
		x_ = CoorT::scale(s, x_);
		y_ = CoorT::scale(s, y_);
		z_ = CoorT::scale(s, z_);
	}


	const char* Tuple3
	::toLog() const {
		sprintf(log_msg(), "(%f, %f, %f)", x_, y_, z_);
		return log_msg();
	}


}
