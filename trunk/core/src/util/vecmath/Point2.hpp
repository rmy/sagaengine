/*
   Copyright (C) 1997,1998,1999
   Kenji Hiranabe, Eiwa System Management, Inc.

   This program is free software.
   Implemented by Kenji Hiranabe(hiranabe@esm.co.jp),
   conforming to the Java(TM) 3D API specification by Sun Microsystems.

   Permission to use, copy, modify, distribute and sell this software
   and its documentation for any purpose is hereby granted without fee,
   provided that the above copyright notice appear in all copies and
   that both that copyright notice and this permission notice appear
   in supporting documentation. Kenji Hiranabe and Eiwa System Management,Inc.
   makes no representations about the suitability of this software for any
   purpose.  It is provided "AS IS" with NO WARRANTY.

   Copyright (C) 2004
   Rune Myrland, Skalden Studio AS
   SagaEngine adaptions. Kenji Hiranabe's license apply.
*/
#ifndef base_vecmath_Point2_hpp
#define base_vecmath_Point2_hpp

#include "util_vecmath.hpp"
#include "Vector2.hpp"
#include "Point3.hpp"

namespace se_core {
	/**
	 * A 2 element point that is represented by x,y coordinates.
	 * @version specification 1.1, implementation $Revision: 1.4 $, $Date: 1999/10/06 02:52:46 $
	 * @author Kenji hiranabe
	 */
	class _SeCoreExport Point2 : public Vector2 {
	public:
		/**
		 * Constructs and initializes a Point2 from the specified xy coordinates.
		 * @param x the x coordinate
		 * @param y the y coordinate
		 */
		Point2(coor_t x, coor_t y): Vector2(x, y) { }

		/**
		 * Constructs and initializes a Point2 from the specified array.
		 * @param p the array of length 2 containing xy in order
		 */
		Point2(const coor_t p[]): Vector2(p) { }

		/**
		 * Constructs and initializes a Point2 from the specified Tuple2.
		 * @param t1 the Tuple2 containing the initialization x y data
		 */
		Point2(const Tuple2& t1): Vector2(t1) { }

		/**
		 * Constructs and initializes a Point2 to (0,0).
		 */
		Point2(): Vector2() { }


		/**
		 * Project 3d point into isometric 2d coordinates.
		 */
		inline void projectIsometric(Point3& p1) {
			x_ = p1.x_;
			y_ = CoorT::half(p1.z_) + CoorT::half(p1.y_);
		}


		/**
		 * Project 3d point into top down 2d coordinates.
		 */
		inline void projectTopDown(Point3& p1) {
			x_ = p1.x_;
			y_ = p1.z_;
		}


		/**
		 * Computes the square of the distance between this point and point p1.
		 * @param  p1 the other point
		 */
		coor_double_t distanceSquared(const Point2& p1) const {
			coor_double_t dx = x_ - p1.x_;
			coor_double_t dy = y_ - p1.y_;
			return static_cast<coor_double_t>(dx)*dx + static_cast<coor_double_t>(dy)*dy;
		}

		/**
		 * Computes the distance between this point and point p1.
		 * @param p1 the other point
		 */
		coor_t distance(const Point2& p1) const;

		/**
		 * Computes the L-1 (Manhattan) distance between this point and point p1.
		 * The L-1 distance is equal to abs(x1-x2) + abs(y1-y2).
		 * @param p1 the other point
		 */
		coor_t distanceL1(const Point2& p1) const;

		/**
		 * Computes the L-infinite distance between this point and point p1.
		 * The L-infinite distance is equal to MAX[abs(x1-x2), abs(y1-y2)].
		 * @param p1 the other point
		 */
		coor_t distanceLinf(const Point2& p1) const;

		scale_t lineIntersect(const Point2& a0, const Point2& a1, const Point2& b0, const Point2& b1);
		scale_t lineIntersect(const Point3& a0, const Point3& a1, const Point3& b0, const Point3& b1);

		bool willAIntersectB(const Point2& a0, const Point2& a1, const Point2& b0, const Point2& b1);
		bool willAIntersectB(const Point3& a0, const Point3& a1, const Point3& b0, const Point3& b1);

		// copy constructor and operator = is made by complier

		Point2& operator=(const Tuple2& t) {
			Tuple2::operator=(t);
			return *this;
		}
	};

} // Namespace


inline
se_core::Point2 operator*(scale_t s, const se_core::Point2& t1) {
    return operator*(s, (const se_core::Tuple2&)t1);
}


#endif /* POINT2_H */
