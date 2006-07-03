#include "Point4.hpp"
#include "../math/CoorT.hpp"


namespace se_core {

	coor_t Point4
	::distance(const Point4& p1) const {
		return CoorT::sqrt(distanceSquared(p1));
	}


	coor_t Point4
	::distanceL1(const Point4& p1) const {
		return CoorT::abs(x-p1.x) + CoorT::abs(y-p1.y)
			+ CoorT::abs(z-p1.z) + CoorT::abs(w-p1.w);
	}


	coor_t Point4
	::distanceLinf(const Point4& p1) const {
		return CoorT::max(CoorT::abs(x-p1.x), CoorT::abs(y-p1.y), CoorT::abs(z-p1.z), CoorT::abs(w-p1.w));
	}

}
