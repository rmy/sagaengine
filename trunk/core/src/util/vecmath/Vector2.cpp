#include "Vector2.hpp"

#include "../math/CoorT.hpp"
#include "../math/BrayT.hpp"
#include "../math/Trig.hpp"

namespace se_core {

	coor_t Vector2
	::length() const {
		return CoorT::sqrt(lengthSquared());
	}


	bray_t Vector2
	::angle(const Vector2& v1) const {
		// stabler than acos
		return BrayT::abs(Trig::atan2(x_*v1.y_ - y_*v1.x_ , dot(v1)));
	}

}
