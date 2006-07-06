#include "Tuple4.hpp"
#include "../math/CoorT.hpp"
#include <cstdio>

namespace se_core {

	bool Tuple4
	::epsilonEquals(const Tuple4& t1, coor_t epsilon) const {
		return (CoorT::abs(t1.x - x) <= epsilon) &&
			(CoorT::abs(t1.y - y) <= epsilon) &&
			(CoorT::abs(t1.z - z) <= epsilon) &&
			(CoorT::abs(t1.w - w) <= epsilon);
	}


	char* Tuple4
	::toString(char* buffer) const {
		sprintf(buffer, "(%f,%f,%f,%f)", x * 1.0, y * 1.0, z * 1.0, w * 1.0);
		return buffer;
	}
}
