#include "Vector4.hpp"
#include "../math/CoorT.hpp"

namespace se_core {

	coor_t Vector4
	::length() const {
		return CoorT::sqrt(lengthSquared());
	}

}
