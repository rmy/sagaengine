#include "Point2.hpp"
#include "Point3.hpp"
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


	/*
	void Point2
	::lineIntersect(const Point2& p0
					   , const Point2& p1
					   , const Point2& p2
					   , const Point2& p3) {
		// this function computes the intersection of the sent lines
		// and returns the intersection point, note that the function assumes
		// the lines intersect. the function can handle vertical as well
		// as horizontal lines. note the function isn't very clever, it simply
		//applies the math, but we don't need speed since this is a
		//pre-processing step

		// b1 and b2 unused
		float a1, c1, // constants of linear equations
			a2, c2,
			det_inv,  // the inverse of the determinant of the coefficie matrix
			m1, m2;    // the slopes of each line

		// compute slopes, note the cludge for infinity, however, this will
		// be close enough

		if ((p1.x_ - p0.x_) != 0)
			m1 = (p1.y_ - p0.y_) / (p1.x_ - p0.x_);
		else
			m1 = (float)1e+10;   // close enough to infinity

		if ((p3.x_ - p2.x_) != 0)
			m2 = (p3.y_ - p2.y_) / (p3.x_ - p2.x_);
		else
			m2 = (float)1e+10;   // close enough to infinity

		// compute constants
		a1 = m1;
		a2 = m2;

		c1 = (p0.y_ - m1 * p0.x_);
		c2 = (p2.y_ - m2 * p2.x_);

		// compute the inverse of the determinate
		det_inv = 1 / (a2 - a1);

		// use Kramers rule to compute xi and yi
		x_ = ((c1 - c2) * det_inv);
		y_ = m1 * ( (c2 - c1) / (m1 - m2) ) + c1;
	}
	*/


	scale_t Point2
	::lineIntersect(const Point2& a0
					   , const Point2& a1
					   , const Point2& b0
					   , const Point2& b1) {
		scale_t den = ((a1.x_-a0.x_) * (b1.y_-b0.y_)-(a1.y_-a0.y_) * (b1.x_-b0.x_));
		if(den == 0) {
			return -1;
		}
		scale_t r = ((a0.y_ - b0.y_) * (b1.x_ - b0.x_) - (a0.x_ - b0.x_) * (b1.y_ - b0.y_)) / den;
		scale_t s = ((a0.y_-b0.y_)*(a1.x_-a0.x_)-(a0.x_-b0.x_)*(a1.y_-a0.y_)) / den;

		if(s < 0 || s > 1 || r < 0 || r > 1) {
			return -1;
		}

		x_ = a0.x_ + r * (a1.x_ - a0.x_);
		y_ = a0.y_ + r * (a1.y_ - a0.y_);

		return r;
	}


	scale_t Point2
	::lineIntersect(const Point3& a0
					   , const Point3& a1
					   , const Point3& b0
					   , const Point3& b1) {
		scale_t den = ((a1.x_-a0.x_) * (b1.z_-b0.z_)-(a1.z_-a0.z_) * (b1.x_-b0.x_));
		if(den == 0)
			return -1;
		scale_t r = ((a0.z_ - b0.z_) * (b1.x_ - b0.x_) - (a0.x_ - b0.x_) * (b1.z_ - b0.z_)) / den;
		scale_t s = ((a0.z_-b0.z_)*(a1.x_-a0.x_)-(a0.x_-b0.x_)*(a1.z_-a0.z_)) / den;

		if(s < 0 || s > 1 || r < 0 || r > 1) {
			return -1;
		}

		x_ = a0.x_ + r * (a1.x_ - a0.x_);
		y_ = a0.z_ + r * (a1.z_ - a0.z_);

		return r;
	}
}
