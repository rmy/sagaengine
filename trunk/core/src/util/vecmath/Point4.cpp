#include "Point4.hpp"
#include "../math/CoorT.hpp"


namespace se_core {

	coor_t Point4
	::distance(const Point4& p1) const {
		return CoorT::sqrt(distanceSquared(p1));
	}


	coor_t Point4
	::distanceL1(const Point4& p1) const {
		return CoorT::abs(x_-p1.x_) + CoorT::abs(y_-p1.y_)
			+ CoorT::abs(z_-p1.z_) + CoorT::abs(w_-p1.w_);
	}


	coor_t Point4
	::distanceLinf(const Point4& p1) const {
		return CoorT::max(CoorT::abs(x_-p1.x_), CoorT::abs(y_-p1.y_), CoorT::abs(z_-p1.z_), CoorT::abs(w_-p1.w_));
	}

}
