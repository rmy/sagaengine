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
		sprintf(buffer, "(%d,%d)", x_, y_);
		return buffer;
	}

}
