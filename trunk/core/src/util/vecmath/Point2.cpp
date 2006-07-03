#include "Point2.hpp"
#include "../math/CoorT.hpp"

namespace se_core {

	coor_t Point2
	::distance(const Point2& p1) const {
		return CoorT::sqrt(distanceSquared(p1));
	}

	coor_t Point2
	::distanceL1(const Point2& p1) const {
		return CoorT::abs(x_-p1.x_) + CoorT::abs(y_-p1.y_);
	}


	coor_t Point2
	::distanceLinf(const Point2& p1) const {
		return CoorT::max(CoorT::abs(x_-p1.x_), CoorT::abs(y_-p1.y_));
	}



}
