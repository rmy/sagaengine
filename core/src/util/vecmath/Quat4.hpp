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

   Copyright (C) 2006
   Rune Myrland, Skalden Studio AS
   SagaEngine adaptions, involving conversion from floating point values to fixed point values.
   Kenji Hiranabe's license apply.
*/
#ifndef util_vecmath_Quat4_hpp
#define util_vecmath_Quat4_hpp

#include "Tuple4.hpp"
#include "../math/CoorT.hpp"
#include "../math/QuatT.hpp"

namespace se_core {
	/**
	 * A 4 element quaternion represented by x,y,z,w coordinates.
	 * @version specification 1.1, implementation $Revision: 1.3 $, $Date: 1999/10/06 02:52:46 $
	 * @author Kenji hiranabe
	 */

	class Quat4 : public Tuple4 {
	public:
		/**
		 * Constructs and initializes a Quat4 from the specified xyzw coordinates.
		 * @param x the x coordinate
		 * @param y the y coordinate
		 * @param z the z coordinate
		 * @param w the w scalar component
		 */
		Quat4(coor_t x, coor_t y, coor_t z, coor_t w): Tuple4(x, y, z, w) { }

		/**
		 * Constructs and initializes a Quat4 from the array of length 4.
		 * @param v the array of length 4 containing xyzw in order
		 */
		Quat4(const coor_t v[]) : Tuple4(v) { }


		/**
		 * Constructs and initializes a Quat4 from the specified Tuple4d.
		 * @param t1 the Tuple4d containing the initialization x y z w data
		 */
		Quat4(const Tuple4& t1): Tuple4(t1) { }

		/**
		 * Constructs uninitialized Quat4.
		 */
		Quat4(): Tuple4() { }

		/**
		 * Sets the value of this tuple to the value of tuple t1.
		 * note: other set methods hides this set.
		 * @param t1 the tuple to be copied
		 */
		void set(const Tuple4& t1) {
			Tuple4::set(t1);
		}


		void setIdentity() {
			set(0, 0, 0, 1);
		}

		/**
		 * Sets the value of this tuple to the specified xyzw coordinates.
		 * note: other set methods hides this set.
		 * @param x the x coordinate
		 * @param y the y coordinate
		 * @param z the z coordinate
		 * @param w the w coordinate
		 */
		void set(coor_t x, coor_t y, coor_t z, coor_t w) {
			Tuple4::set(x, y, z, w);
		}


		/**
		 * Sets the value of this quaternion to the conjugate of quaternion q1.
		 * @param q1 the source vector
		 */
		void conjugate(const Quat4& q1) {
			x_ = -q1.x_;
			y_ = -q1.y_;
			z_ = -q1.z_;
			w_ = q1.w_;
		}

		/**
		 * Negate the value of of each of this quaternion's x,y,z coordinates
		 *  in place.
		 */
		void conjugate() {
			x_ = -x_;
			y_ = -y_;
			z_ = -z_;
		}

		/**
		 * Sets the value of this quaternion to the quaternion product of
		 * quaternions q1 and q2 (this = q1 * q2).
		 * Note that this is safe for aliasing (e.g. this can be q1 or q2).
		 * @param q1 the first quaternion
		 * @param q2 the second quaternion
		 */
		void mul(const Quat4& q1, const Quat4& q2);

		/**
		 * Sets the value of this quaternion to the quaternion product of
		 * itself and q1 (this = this * q1).
		 * @param q1 the other quaternion
		 */
		void mul(const Quat4& q1);

		/**
		 *
		 * Multiplies quaternion q1 by the inverse of quaternion q2 and places
		 * the value into this quaternion.  The value of both argument quaternions
		 * is preservered (this = q1 * q2^-1).
		 * @param q1 the left quaternion
		 * @param q2 the right quaternion
		 */
		void mulInverse(const Quat4& q1, const Quat4& q2);

		/**
		 * Multiplies this quaternion by the inverse of quaternion q1 and places
		 * the value into this quaternion.  The value of the argument quaternion
		 * is preserved (this = this * q^-1).
		 * @param q1 the other quaternion
		 */
		void mulInverse(const Quat4& q1);

	protected:
		coor_double_t norm() const {
			return QuatT::pow2(x_) + QuatT::pow2(y_) + QuatT::pow2(z_) + QuatT::pow2(w_);
		}

	public:
		/**
		 * Sets the value of this quaternion to quaternion inverse of
		 * quaternion q1.
		 * @param q1 the quaternion to be inverted
		 */
		void inverse(const Quat4& q1) {
			scale_t n = QuatT::oneOver( q1.norm() );
			// zero-div may occur.
			x_ = -QuatT::scale(n, q1.x_);
			y_ = -QuatT::scale(n, q1.y_);
			z_ = -QuatT::scale(n, q1.z_);
			w_ = QuatT::scale(n, q1.w_);
		}

		/**
		 * Sets the value of this quaternion to the quaternion inverse
		 * of itself.
		 */
		void inverse() {
			scale_t n = QuatT::oneOver( norm() );
			// zero-div may occur.

			x_ = -QuatT::scale(n, x_);
			y_ = -QuatT::scale(n, y_);
			z_ = -QuatT::scale(n, z_);
			w_ = QuatT::scale(n, w_);
		}

		/**
		 * Sets the value of this quaternion to the normalized value
		 * of quaternion q1.
		 * @param q1 the quaternion to be normalized.
		 */
		void normalize(const Quat4& q1) {
			scale_t n = QuatT::oneOver( CoorT::sqrt( q1.norm() ) );
			// zero-div may occur.
			x_ = QuatT::scale(n, q1.x_);
			y_ = QuatT::scale(n, q1.y_);
			z_ = QuatT::scale(n, q1.z_);
			w_ = QuatT::scale(n, q1.w_);
		}

		/**
		 * Normalizes the value of this quaternion in place.
		 */
		void normalize() {
			scale_t n = QuatT::oneOver( CoorT::sqrt( norm() ) );
			// zero-div may occur.

			x_ = QuatT::scale(n, x_);
			y_ = QuatT::scale(n, y_);
			z_ = QuatT::scale(n, z_);
			w_ = QuatT::scale(n, w_);
		}


		bool isNormalized() const {
            #ifdef SE_FIXED_POINT
			LogFatal("Not yet implemented");
			return true;
            #else
			coor_double_t n = norm();
			const coor_double_t EPSILON = 0.00001;
			return (n + EPSILON > 1 && n - EPSILON < 1);
			#endif
		}

		/**
		 * Sets the value of this quaternion to the equivalent rotation of teh
		 * AxisAngle argument.
		 * @param a1 the axis-angle
		 */
		//void set(const AxisAngle4<coor_t>& a1);

		void setEuler(const Euler3& a1);
		void setEuler(const bray_t yaw);
		void setEuler(const bray_t yaw, const bray_t pitch);
		
		/**
		 * Performs a great circle interpolation between this quaternion
		 * and the quaternion parameter and places the result into this
		 * quaternion.
		 * @param q1 the other quaternion
		 * @param alpha the alpha interpolation parameter
		 */
		void slerp(const Quat4& q1, scale_t alpha, bool findShortestPath = false);

		/**
		 * Performs a great circle interpolation between quaternion q1 and
		 * quaternion q2 and places the result into this quaternion.
		 * @param q1 the first quaternion
		 * @param q2 the second quaternion
		 * @param alpha the alpha interpolation parameter
		 */
		void slerp(const Quat4& q1, const Quat4& q2, coor_t alpha, bool findShortestPath = false);

		// copy constructor and operator = is made by complier

		//Quat4& operator*=(const Quat4& m1);
		//Quat4 operator*(const Quat4& m1) const;


		static const Quat4 IDENTITY;
	};


}



#endif /* QUAT4__H */
