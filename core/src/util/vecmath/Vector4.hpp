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
#ifndef base_vecmath_Vector4_hpp
#define base_vecmath_Vector4_hpp

#include "Tuple4.hpp"
#include "Tuple3.hpp"

namespace se_core {

	/**
	 * A 4 element vector that is represented by x,y,z,w coordinates.
	 * @version specification 1.1, implementation $Revision: 1.3 $, $Date: 1999/10/06 02:52:46 $
	 * @author Kenji hiranabe
	 */
	class Vector4 : public Tuple4 {
	public:
		/**
		 * Constructs and initializes a Vector4 from the specified xyzw coordinates.
		 * @param x the x coordinate
		 * @param y the y coordinate
		 * @param z the z coordinate
		 * @param w the w coordinate
		 */
		Vector4(coor_t x, coor_t y, coor_t z, coor_t w): Tuple4(x, y, z, w) { }

		/**
		 * Constructs and initializes a Vector4 from the specified array of length 4.
		 * @param v the array of length 4 containing xyzw in order
		 */
		Vector4(const coor_t v[]): Tuple4(v) { }

		/**
		 * Constructs and initializes a Vector4 from the specified Tuple4d.
		 * @param t1 the Tuple4d containing the initialization x y z w data
		 */
		Vector4(const Tuple4& t1): Tuple4(t1) { }

		/**
		 * Constructs and initializes a Vector4 to (0,0,0,0).
		 */
		Vector4(): Tuple4() { }

		/**
		 * Constructs and initializes a Vector4 from the specified Tuple3.
		 * The x,y,z  components of this point are set to the corresponding
		 * components
		 * of tuple t1. The w component of this point is set to 0.
		 *
		 * @param t1 the tuple to be copied
		 * @since Java3D 1.2
		 */
		Vector4(const Tuple3& t1): Tuple4(t1.x_, t1.y_, t1.z_, 0) { }

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
			w_ = 0;
		}

		/**
		 * Returns the squared length of this vector.
		 * @return the squared length of this vector
		 */
		coor_double_t lengthSquared() const {
			return static_cast<coor_double_t>(x_)*x_
				+ static_cast<coor_double_t>(y_)*y_
				+ static_cast<coor_double_t>(z_)*z_
				+ static_cast<coor_double_t>(w_)*w_;
		}

		/**
		 * Returns the length of this vector.
		 * @return the length of this vector
		 */
		coor_t length() const;


		/**
		 * Computes the dot product of the this vector and vector v1.
		 * @param  v1 the other vector
		 * @return the dot product of this vector and vector v1
		 */
		coor_t dot(const Vector4& v1) const {
			return CoorT::fromd
				((static_cast<coor_double_t>(x_) * v1.x_)
				+ (static_cast<coor_double_t>(y_) * v1.y_)
				+ (static_cast<coor_double_t>(z_) * v1.z_)
				+ (static_cast<coor_double_t>(w_) * v1.w_));
		}

		/**
		 * Sets the value of this vector to the normalization of vector v1.
		 * @param v1 the un-normalized vector
		 */
		void normalize(const Vector4& v1) {
			Tuple4::set(v1);
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
			w_ = CoorT::fromd(static_cast<coor_double_t>(w_)) / d;
		}

		/**
		 * Returns the (4-space) angle in radians between this vector and
		 * the vector parameter; the return value is constrained to the
		 * range [0,PI].
		 * @param v1  the other vector
		 * @return the angle in radians in the range [0,PI]
		 */
		bray_t angle(const Vector4& v1) const {
			LogFatal("Not implmented");
			/*
			// zero div may occur.
			coor_t d = dot(v1);
			coor_t v1_length = v1.length();
			coor_t v_length = length();

			// numerically, domain error may occur
			return T(acos(d/v1_length/v_length));
			*/
			return 0;
		}

		// copy constructor and operator = is made by complier

		Vector4& operator=(const Tuple4& t) {
			Tuple4::operator=(t);
			return *this;
		}
	};

} // Namespace

template <class T>
inline
se_core::Vector4 operator*(scale_t s, const se_core::Vector4& t1) {
    return operator*(s, (const se_core::Tuple4&)t1);
}

#endif /* VECTOR4_H */
