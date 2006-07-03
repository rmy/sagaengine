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
   SagaEngine adaptions, involving conversion from floating point values to fixed point values.
   Kenji Hiranabe's license apply.
*/
#ifndef base_vecmath_Point3_hpp
#define base_vecmath_Point3_hpp

#include "util_vecmath.hpp"
#include "Tuple3.hpp"

namespace se_core {
	/**
	 * A 3 element point that is represented by x,y,z coordinates.
	 * @author Kenji hiranabe, Rune Myrland
	 */
	class Point3 : public Tuple3 {
	public:
		/**
		 * Constructs and initializes a Point3 from the specified xyz coordinates.
		 * @param x the x coordinate
		 * @param y the y coordinate
		 * @param z the z coordinate
		 */
		Point3(coor_t x, coor_t y, coor_t z): Tuple3(x, y, z) { }

		/**
		 * Constructs and initializes a Point3 from the specified array.
		 * @param p the array of length 3 containing xyz in order
		 */
		Point3(const coor_t p[]): Tuple3(p) { }

		/*
		 * Constructs and initializes a Point3 from the specified Tuple3d.
		 * @param t1 the Tuple3d containing the initialization x y z data
		 */
		Point3(const Tuple3& t1): Tuple3(t1) { }

		/**
		 * Constructs and initializes a Point3 to (0,0,0).
		 */
		Point3(): Tuple3() { }

		/**
		 * Computes the square of the distance between this point and point p1.
		 * @param  p1 the other point
		 * @return the square of distance between these two points as a float
		 */
		coor_double_t distanceSquared(const Point3& p1) const {
			coor_double_t dx = x_ - p1.x_;
			coor_double_t dy = y_ - p1.y_;
			coor_double_t dz = z_ - p1.z_;
			return dx*dx + dy*dy + dz*dz;
		}

		/**
		 * Returns the distance between this point and point p1.
		 * @param p1 the other point
		 * @return the distance between these two points as a float
		 */
		coor_t distance(const Point3& p1) const;

		/**
		 * Computes the L-1 (Manhattan) distance between this point and point p1.
		 * The L-1 distance is equal to abs(x1-x2) + abs(y1-y2).
		 * @param p1 the other point
		 */
		coor_t distanceL1(const Point3& p1) const;

		/**
		 * Computes the L-infinite distance between this point and point p1.
		 * The L-infinite distance is equal to MAX[abs(x1-x2), abs(y1-y2)].
		 * @param p1 the other point
		 */
		coor_t distanceLinf(const Point3& p1) const;

		/**
		 * Multiplies each of the x,y,z components of the Point4 parameter
		 * by 1/w and places the projected values into this point.
		 * @param p1 the source Point4, which is not modified
		 */
		void project(const Point4& p1);

		// copy constructor and operator = is made by complier

		Point3& operator=(const Tuple3& t) {
			Tuple3::operator=(t);
			return *this;
		}
	};

} // Namespace

inline
se_core::Point3 operator*(scale_t s, const se_core::Point3& t1) {
    return operator*(s, (const se_core::Tuple3&)t1);
}


#endif /* POINT3_H */
