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
*/
#ifndef base_vecmath_Point4_hpp
#define base_vecmath_Point4_hpp

#include "Vector4.hpp"
#include "Tuple3.hpp"

namespace se_core {

	/**
	 * A 4 element point that is represented x,y,z,w coordinates.
	 * @version specification 1.1, implementation $Revision: 1.3 $, $Date: 1999/10/06 02:52:46 $
	 * @author Kenji hiranabe
	 */
	class _SeCoreExport Point4 : public Vector4 {
	public:
		/**
		 * Constructs and initializes a Point4 from the specified xyzw coordinates.
		 * @param x the x coordinate
		 * @param y the y coordinate
		 * @param z the z coordinate
		 * @param w the w coordinate
		 */
		Point4(coor_t x, coor_t y, coor_t z, coor_t w): Vector4(x, y, z, w) { }

		/**
		 * Constructs and initializes a Point4 from the specified array.
		 * @param p the array of length 4 containing xyzw in order
		 */
		Point4(const coor_t p[]): Vector4(p) { }

		/**
		 * Constructs and initializes a Point4 from the specified Tuple4d.
		 * @param t1 the Tuple4d containing the initialization x y z w data
		 */
		Point4(const Tuple4& t1): Vector4(t1) { }

		/**
		 * Constructs and initializes a Point4 to (0,0,0,0).
		 */
		Point4(): Vector4() { }

		/**
		 * Constructs and initializes a Point4 from the specified Tuple3.
		 * The x,y,z  components of this point are set to the corresponding
		 * components
		 * of tuple t1. The w component of this point is set to 1.
		 *
		 * @param t1 the tuple to be copied
		 * @since Java3D 1.2
		 */
		Point4(const Tuple3& t1): Vector4(t1.x_, t1.y_, t1.z_, 1) { }

		/**
		 * Sets the x,y,z components of this point to the corresponding
		 * components of tuple t1. The w component of this point is set to 1.
		 *
		 * NOTE:
		 * This method's name should be 'set'. Newer compilers support
		 * 'using Tuple4::set;' and use the name 'set'.
		 * but for now, renamed to 'set3'.
		 *
		 * @param t1 the tuple to be copied
		 * @since Java3D 1.2
		 */
		void set3(const Tuple3& t1) {
			x_ = t1.x_;
			y_ = t1.y_;
			z_ = t1.z_;
			w_ = 1;
		}

		/**
		 * Computes the square of the distance between this point and point p1.
		 * @param  p1 the other point
		 * @return the square of distance between this point and p1
		 */
		coor_double_t distanceSquared(const Point4& p1) const {
			coor_double_t dx = x_ - p1.x_;
			coor_double_t dy = y_ - p1.y_;
			coor_double_t dz = z_ - p1.z_;
			coor_double_t dw = w_ - p1.w_;
			return dx*dx + dy*dy + dz*dz + dw*dw;
		}

		/**
		 * Returns the distance between this point and point p1.
		 * @param p1 the other point
		 * @return the distance between this point and point p1.
		 */
		coor_t distance(const Point4& p1) const;


		/**
		 * Computes the L-1 (Manhattan) distance between this point and point p1.
		 * The L-1 distance is equal to abs(x1-x2) + abs(y1-y2)
		 * + abs(z1-z2) + abs(w1-w2).
		 * @param p1 the other point
		 * @return L-1 distance
		 */
		coor_t distanceL1(const Point4& p1) const;


		/**
		 * Computes the L-infinite distance between this point and point p1.
		 * The L-infinite distance is equal to MAX[abs(x1-x2), abs(y1-y2), abs(z1-z2), abs(w1-w2)].
		 * @param p1 the other point
		 * @return L-infinite distance
		 */
		coor_t distanceLinf(const Point4& p1) const;


		/**
		 * Multiplies each of the x,y,z components of the Point4 parameter by 1/w,
		 * places the projected values into this point, and places a 1 as the w
		 * parameter of this point.
		 * @param p1 the source Point4, which is not modified
		 */
		void project(const Point4& p1) {
			// zero div may occur.
			x_ = CoorT::div(p1.x_, p1.w_);
			y_ = CoorT::div(p1.y_, p1.w_);
			z_ = CoorT::div(p1.z_, p1.w_);
			w_ = 1;
		}

		// copy constructor and operator = is made by complier

		Point4& operator=(const Tuple4& t) {
			Tuple4::operator=(t);
			return *this;
		}
	};

} // Namespace

inline
se_core::Point4 operator*(scale_t s, const se_core::Point4& t1) {
    return operator*(s, (const se_core::Tuple4&)t1);
}


#endif /* POINT4_H */

