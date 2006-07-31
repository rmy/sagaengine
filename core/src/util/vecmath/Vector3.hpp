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
#ifndef base_vecmath_Vector3_hpp
#define base_vecmath_Vector3_hpp

#include "util_vecmath.hpp"
#include "Tuple3.hpp"

namespace se_core {
	/**
	 * A 3 element vector that is represented by
	 * x,y,z coordinates. If this value represents a normal, then it should be
	 * normalized.
	 * @author Kenji hiranabe, Rune Myrland
	 */
	class Vector3 : public Tuple3 {
	public:
		/**
		 * Constructs and initializes a Vector3 from the specified xyz coordinates.
		 * @param x the x coordinate
		 * @param y the y coordinate
		 * @param z the z coordinate
		 */
		Vector3(coor_t x, coor_t y, coor_t z): Tuple3(x, y, z) { }

		/**
		 * Constructs and initializes a Vector3 from the specified array of length 3.
		 * @param v the array of length 3 containing xyz in order
		 */
		Vector3(const coor_t v[]): Tuple3(v) {  }

		/**
		 * Constructs and initializes a Vector3 from the specified Tuple3d.
		 * @param t1 the Tuple3d containing the initialization x y z data
		 */
		Vector3(const Tuple3& t1): Tuple3(t1) { }

		/**
		 * Constructs and initializes a Vector3 to (0,0,0).
		 */
		Vector3(): Tuple3() { }

		/**
		 * Sets this vector to be the vector cross product of vectors v1 and v2.
		 * @param v1 the first vector
		 * @param v2 the second vector
		 */
		void cross(const Vector3& v1, const Vector3& v2) {
			// store on stack once for aliasing-safty
			// i.e. safe when a.cross(a, b)
			set(
				CoorT::fromd(static_cast<coor_double_t>(v1.y_)*v2.z_ - static_cast<coor_double_t>(v1.z_)*v2.y_),
				CoorT::fromd(static_cast<coor_double_t>(v1.z_)*v2.x_ - static_cast<coor_double_t>(v1.x_)*v2.z_),
				CoorT::fromd(static_cast<coor_double_t>(v1.x_)*v2.y_ - static_cast<coor_double_t>(v1.y_)*v2.x_)
				);
		}

		/**
		 * Sets the value of this vector to the normalization of vector v1.
		 * @param v1 the un-normalized vector
		 */
		void normalize(const Vector3& v1) {
			set(v1);
			normalize();
		}

		/**
		 * Normalizes this vector in place.
		 */
		void normalize() {
			coor_t d = length();

			// zero-div may occur.
			x_ = CoorT::fromd(static_cast<coor_double_t>(x_)) / d;
			y_ = CoorT::fromd(static_cast<coor_double_t>(y_)) / d;
			z_ = CoorT::fromd(static_cast<coor_double_t>(z_)) / d;
		}

		/**
		 * Computes the dot product of the this vector and vector v1.
		 * @param  v1 the other vector
		 */
		coor_t dot(const Vector3& v1) const {
			return
				CoorT::fromd((static_cast<coor_double_t>(x_)*v1.x_)
				+ (static_cast<coor_double_t>(y_)*v1.y_)
				+ (static_cast<coor_double_t>(z_)*v1.z_));
		}

		/**
		 * Returns the squared length of this vector.
		 * @return the squared length of this vector
		 */
		coor_double_t lengthSquared() const {
			return
				(static_cast<coor_double_t>(x_)*x_)
				+ (static_cast<coor_double_t>(y_)*y_)
				+ (static_cast<coor_double_t>(z_)*z_);
		}

		/**
		 * Returns the length of this vector.
		 * @return the length of this vector
		 */
		coor_t length() const;


		/**
		 * Returns the angle in radians between this vector and
		 * the vector parameter; the return value is constrained to the
		 * range [0,PI].
		 * @param v1  the other vector
		 * @return the angle in radians in the range [0,PI]
		 */
		bray_t angle(const Vector3& v1) const;


		void rotate(const Quat4& q);
		void rotate(const Vector3& v, const Quat4& q);
		void rotateInverse(const Quat4& q);

		void rotate(const Euler3& a);
		void rotate(const Vector3& v, const Euler3& a);
		void rotateInverse(const Euler3& a);

		void setForward(const coor_t len, const bray_t yaw);
		void setForward(const coor_t len, const bray_t yaw, const bray_t pitch);
		void setForward(const coor_t len, const Euler3& a1);


		// copy constructor and operator = is made by complier

		Vector3& operator=(const Tuple3& t) {
			Tuple3::operator=(t);
			return *this;
		}

		static const Vector3 ZERO;
		static const Vector3 UNIT_FORWARD;

	};

} // Namespace


inline
se_core::Vector3 operator*(scale_t s, const se_core::Vector3& t1) {
    return operator*(s, (const se_core::Tuple3&)t1);
}

#endif /* VECTOR3_H */
