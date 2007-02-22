#include "Tuple2.hpp"
#include "../math/CoorT.hpp"
#include <cstdio>

namespace se_core {

	bool Tuple2
	::epsilonEquals(const Tuple2& t1, coor_t epsilon) const {
		return (CoorT::abs(t1.x_ - x_) <= epsilon) &&
			(CoorT::abs(t1.y_ - y_) <= epsilon);
	}


	char* Tuple2
	::toString(char* buffer) const {
		sprintf(buffer, "(%f,%f)", x_ * 1.0, y_ * 1.0);
		return buffer;
	}

	bool Tuple2
	::isNan() const {
		return x_ != x_ || y_ != y_;
	}
}
