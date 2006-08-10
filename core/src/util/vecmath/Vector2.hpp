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
#ifndef base_vecmath_Vector2_hpp
#define base_vecmath_Vector2_hpp

#include "Tuple2.hpp"
#include "../math/CoorT.hpp"

namespace se_core {
	/**
	 * A 2 element vector that is represented by x,y coordinates.
	 * Java3D specification 1.1, implementation $Revision: 1.3 $, $Date: 1999/10/06 02:52:46 $
	 * @author Kenji hiranabe
	 * SagaEngine adaption
	 * @author Rune Myrland
	 */

	class _SeCoreExport Vector2 : public Tuple2 {
	public:
		/**
		 * Constructs and initializes a Vector2 from the specified xy coordinates.
		 * @param x the x coordinate
		 * @param y the y coordinate
		 */
		Vector2(coor_t x, coor_t y) : Tuple2(x, y) { }

		/**
		 * Constructs and initializes a Vector2 from the specified array.
		 * @param v the array of length 2 containing xy in order
		 */
		Vector2(const coor_t v[]) : Tuple2(v) {  }

		/**
		 * Constructs and initializes a Vector2 from the specified Tuple2.
		 * @param t1 the Tuple2 containing the initialization x y data
		 */
		Vector2(const Tuple2& t1) : Tuple2(t1) { }

		/**
		 * Constructs and initializes a Vector2 to (0,0).
		 */
		Vector2(): Tuple2() { }

		/**
		 * Computes the dot product of the this vector and vector v1.
		 * @param  v1 the other vector
		 */
		coor_t dot(const Vector2& v1) const {
			return CoorT::fromd( CoorT::muld(x_, v1.x_) + CoorT::muld(y_, v1.y_) );
		}

		/**
		 * Returns the squared length of this vector.
		 * @return the squared length of this vector
		 */
		coor_double_t lengthSquared() const {
			return CoorT::muld(x_, x_) + CoorT::muld(y_, y_);
		}

		/**
		 * Returns the length of this vector.
		 * @return the length of this vector
		 */
		coor_t length() const;


		/**
		 * Normalizes this vector in place.
		 */
		void normalize() {
			coor_t d = length();

			// zero-div may occur.
			x_ = CoorT::div(x_, d);
			y_ = CoorT::div(y_, d);
		}

		/**
		 * Sets the value of this vector to the normalization of vector v1.
		 * @param v1 the un-normalized vector
		 */
		void normalize(const Vector2& v1) {
			set(v1);
			normalize();
		}

		/**
		 * Returns the angle in braybrookians between this vector and
		 * the vector parameter; the return value is constrained to the
		 * range [0,255].
		 * @param v1  the other vector
		 * @return the angle in braybrookians in the range [0,255]
		 */
		bray_t angle(const Vector2& v1) const;

		// copy constructor and operator = is made by complier

		Vector2& operator=(const Tuple2& t) {
			Tuple2::operator=(t);
			return *this;
		}
	};
} // Namespace

inline
se_core::Vector2 operator*(scale_t s, const se_core::Vector2& t1) {
    return operator*(s, (const se_core::Tuple2&)t1);
}

#endif /* VECTOR2_H */
