#include "Point3.hpp"
#include "Point4.hpp"
#include "../math/CoorT.hpp"

namespace se_core {

	coor_t Point3
	::distance(const Point3& p1) const {
		return CoorT::sqrt(distanceSquared(p1));
	}


	coor_t Point3
	::distanceL1(const Point3& p1) const {
		return CoorT::abs(x_-p1.x_) + CoorT::abs(y_-p1.y_) + CoorT::abs(z_-p1.z_);
	}


	coor_t Point3
	::distanceLinf(const Point3& p1) const {
		return CoorT::max(CoorT::abs(x_-p1.x_), CoorT::abs(y_-p1.y_), CoorT::abs(z_-p1.z_));
	}

	void Point3
	::project(const Point4& p1) {
		// zero div may occur.
		x_ = CoorT::div(p1.x_, p1.w_);
		y_ = CoorT::div(p1.y_, p1.w_);
		z_ = CoorT::div(p1.z_, p1.w_);
	}

}
