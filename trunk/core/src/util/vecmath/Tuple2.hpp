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
#ifndef base_vecmath_Tuple2_hpp
#define base_vecmath_Tuple2_hpp

#include "util_vecmath.hpp"
#include "../error/Log.hpp"
#include "../math/CoorT.hpp"

namespace se_core {

	/**
	 * A generic 2 element tuple that is represented by
	 * point x,y coordinates.
	 * Java3D specification 1.1, implementation $Revision: 1.4 $, $Date: 1999/10/22 08:37:10 $
	 * @author Kenji hiranabe
	 * SagaEngine adaption
	 * @author Rune Myrland
	 */
	class Tuple2 {
	public:
		/**
		 * dimension
		 */
		enum { DIMENSION = 2 };

		/**
		 * The x coordinate.
		 */
		coor_t x_;

		/**
		 * The y coordinate.
		 */
		coor_t y_;

		/**
		 * Constructs and initializes a Tuple from the specified xy coordinates.
		 * @param xvalue the x coordinate
		 * @param yvalue the y coordinate
		 */
		Tuple2(coor_t xvalue, coor_t yvalue): x_(xvalue), y_(yvalue) { }

		/**
		 * Constructs and initializes a Tuple from the specified array.
		 * @param t the array of length 2 containing xy in order
		 */
		Tuple2(const coor_t t[]): x_(t[0]), y_(t[1]) { }


		/**
		 * Constructs and initializes a Tuple2 to (0,0).
		 */
		Tuple2(): x_(0), y_(0) { }

		/**
		 * Sets the value of this tuple to the specified xy coordinates.
		 * @param xvalue the x coordinate
		 * @param yvalue the y coordinate
		 */
		void set(coor_t xvalue, coor_t yvalue) {
			x_ = xvalue;
			y_ = yvalue;
		}

		/**
		 * Sets the value of this tuple from the 2 values specified in the array.
		 * @param t the array of length 2 containing xy in order
		 */
		void set(const coor_t t[]) {
			x_ = t[0];
			y_ = t[1];
		}

		/**
		 * Sets the value of this tuple to the value of the Tuple2 argument.
		 * @param t1 the tuple to be copied
		 */
		void set(const Tuple2& t1) {
			x_ = t1.x_;
			y_ = t1.y_;
		}

		/**
		 * Copies the value of the elements of this tuple into the array t[].
		 * @param t the array that will contain the values of the vector
		 *          asserting t has room for 2 values.
		 */
		void get(coor_t t[]) const {
			t[0] = x_;
			t[1] = y_;
		}

		/**
		 * Gets the value of this tuple and copies the values into the Tuple2.
		 * @param t Tuple2 object into which that values of this object are copied
		 */
		void get(Tuple2* t) const {
			Assert(t);
			t->x_ = x_;
			t->y_ = y_;
		}

		/**
		 * Sets the value of this tuple to the vector sum of tuples t1 and t2.
		 * @param t1 the first tuple
		 * @param t2 the second tuple
		 */
		void add(const Tuple2& t1, const Tuple2& t2) {
			x_ = t1.x_ + t2.x_;
			y_ = t1.y_ + t2.y_;
		}

		/**
		 * Sets the value of this tuple to the vector sum of itself and tuple t1.
		 * @param t1  the other tuple
		 */
		void add(const Tuple2& t1) {
			x_ += t1.x_;
			y_ += t1.y_;
		}

		/**
		 * Sets the value of this tuple to the vector difference of tuple t1 and t2 (this = t1 - t2).
		 * @param t1 the first tuple
		 * @param t2 the second tuple
		 */
		void sub(const Tuple2& t1, const Tuple2& t2) {
			x_ = t1.x_ - t2.x_;
			y_ = t1.y_ - t2.y_;
		}

		/**
		 * Sets the value of this tuple to the vector difference of itself and tuple t1 (this = this - t1).
		 * @param t1 the other tuple
		 */
		void sub(const Tuple2& t1) {
			x_ -= t1.x_;
			y_ -= t1.y_;
		}

		/**
		 * Sets the value of this tuple to the negation of tuple t1.
		 * @param t1 the source vector
		 */
		void negate(const Tuple2& t1) {
			x_ = -t1.x_;
			y_ = -t1.y_;
		}

		/**
		 * Negates the value of this vector in place.
		 */
		void negate() {
			x_ = -x_;
			y_ = -y_;
		}

		/**
		 * Sets the value of this tuple to the scalar multiplication of tuple t1.
		 * @param s the scalar value
		 * @param t1 the source tuple
		 */
		void scale(scale_t s, const Tuple2& t1) {
			x_ = CoorT::scale(s, t1.x_);
			y_ = CoorT::scale(s, t1.y_);
		}

		/**
		 * Sets the value of this tuple to the scalar multiplication of itself.
		 * @param s the scalar value
		 */
		void scale(scale_t s) {
			x_ = CoorT::scale(s, x_);
			y_ = CoorT::scale(s, y_);
		}

		/**
		 * Sets the value of this tuple to the scalar multiplication of tuple t1 and then
		 * adds tuple t2 (this = s*t1 + t2).
		 * @param s the scalar value
		 * @param t1 the tuple to be multipled
		 * @param t2 the tuple to be added
		 */
		void scaleAdd(scale_t s, const Tuple2& t1, const Tuple2& t2) {
			x_ = CoorT::scale(s, t1.x_ + t2.x_);
			y_ = CoorT::scale(s, t1.y_ + t2.y_);
		}

		/**
		 * Sets the value of this tuple to the scalar multiplication of itself and then
		 * adds tuple t1 (this = s*this + t1).
		 * @param s the scalar value
		 * @param t1 the tuple to be added
		 */
		void scaleAdd(scale_t s, const Tuple2& t1) {
			x_ = CoorT::scale(s, x_) + t1.x_;
			y_ = CoorT::scale(s, y_) + t1.y_;
		}

		/**
		 * Returns true if all of the data members of Tuple2 t1 are equal to the corresponding
		 * data members in this
		 * @param t1 the vector with which the comparison is made.
		 */
		bool equals(const Tuple2& t1) const {
			return t1.x_ == x_ && t1.y_ == y_;
		}

		/**
		 * Returns true if the L-infinite distance between this tuple and tuple t1 is
		 * less than or equal to the epsilon parameter, otherwise returns false. The L-infinite
		 * distance is equal to MAX[abs(x1-x2), abs(y1-y2)].
		 * @param t1 the tuple to be compared to this tuple
		 * @param epsilon the threshold value
		 */
		bool epsilonEquals(const Tuple2& t1, coor_t epsilon) const;

		/**
		 * Clamps the tuple parameter to the range [low, high] and places the values
		 * into this tuple.
		 * @param min the lowest value in the tuple after clamping
		 * @param max the highest value in the tuple after clamping
		 * @param t the source tuple, which will not be modified
		 */
		void clamp(coor_t min, coor_t max, const Tuple2& t) {
			set(t);
			clamp(min, max);
		}

		/**
		 * Clamps the minimum value of the tuple parameter to the min parameter
		 * and places the values into this tuple.
		 * @param min the lowest value in the tuple after clamping
		 * @param t the source tuple, which will not be modified
		 */
		void clampMin(coor_t min, const Tuple2& t) {
			set(t);
			clampMin(min);
		}

		/**
		 * Clamps the maximum value of the tuple parameter to the max parameter and
		 * places the values into this tuple.
		 * @param max the highest value in the tuple after clamping
		 * @param t the source tuple, which will not be modified
		 */
		void clampMax(coor_t max, const Tuple2& t) {
			set(t);
			clampMax(max);
		}


		/**
		 * Sets each component of the tuple parameter to its absolute value and
		 * places the modified values into this tuple.
		 * @param t the source tuple, which will not be modified
		 */
		void absolute(const Tuple2& t) {
			set(t);
			absolute();
		}

		/**
		 * Clamps this tuple to the range [low, high].
		 * @param min the lowest value in this tuple after clamping
		 * @param max the highest value in this tuple after clamping
		 */
		void clamp(coor_t min, coor_t max) {
			clampMin(min);
			clampMax(max);
		}

		/**
		 * Clamps the minimum value of this tuple to the min parameter.
		 * @param min the lowest value in this tuple after clamping
		 */
		void clampMin(coor_t min) {
			if (x_ < min)
				x_ = min;
			if (y_ < min)
				y_ = min;
		}

		/**
		 * Clamps the maximum value of this tuple to the max parameter.
		 * @param max the highest value in the tuple after clamping
		 */
		void clampMax(coor_t max) {
			if (x_ > max)
				x_ = max;
			if (y_ > max)
				y_ = max;
		}

		/**
		 * Sets each component of this tuple to its absolute value.
		 */
		void absolute() {
			if (x_ < 0)
				x_ = -x_;
			if (y_ < 0)
				y_ = -y_;
		}

		/**
		 * Linearly interpolates between tuples t1 and t2 and places the
		 * result into this tuple: this = (1-alpha)*t1 + alpha*t2.
		 * @param t1 the first tuple
		 * @param t2 the second tuple
		 * @param alpha the alpha interpolation parameter
		 */
		void interpolate(const Tuple2& t1, const Tuple2& t2, scale_t alpha) {
			set(t1);
			interpolate(t2, alpha);
		}


		/**
		 * Linearly interpolates between this tuple and tuple t1 and places the
		 * result into this tuple: this = (1-alpha)*this + alpha*t1.
		 * @param t1 the first tuple
		 * @param alpha the alpha interpolation parameter
		 *
		 */
		void interpolate(const Tuple2& t1, scale_t alpha) {
			scale_t beta = SCALE_RES - alpha;
			x_ = CoorT::fromScale(beta*x_ + alpha*t1.x_);
			y_ = CoorT::fromScale(beta*y_ + alpha*t1.y_);
		}

		/**
		 * Returns a string that contains the values of this Tuple2. The form is (x,y).
		 * @return the String representation
		 */
		char* toString(char* buffer) const;

		// copy constructor and operator = is made by complier

		bool operator==(const Tuple2& t1) const {
			return equals(t1);
		}

		coor_t operator[](short index) const {
			Assert(index < (short)DIMENSION);
			switch (index) {
			case 0:
				return x_;
			case 1:
				return y_;
			default:
				// error !
				return 0;
			}
		}
		coor_t& operator[](short index) {
			static coor_t dummy;
			Assert(index < (short)DIMENSION);
			switch (index) {
			case 0:
				return x_;
			case 1:
				return y_;
			default:
				// error !
				return dummy;
			}
		}

		Tuple2& operator=(const Tuple2& t1) {
			set(t1);
			return *this;
		}

		Tuple2& operator+=(const Tuple2& t1) {
			add(t1);
			return *this;
		}
		Tuple2& operator-=(const Tuple2& t1) {
			sub(t1);
			return *this;
		}
		Tuple2& operator*=(scale_t s) {
			scale(s);
			return *this;
		}
		Tuple2 operator+(const Tuple2& t1) const {
			return (Tuple2(*this)).operator+=(t1);
		}
		Tuple2 operator-(const Tuple2& t1) const {
			return (Tuple2(*this)).operator-=(t1);
		}
		Tuple2 operator*(scale_t s) const {
			return (Tuple2(*this)).operator*=(s);
		}
	};

} // Namespace

inline
se_core::Tuple2 operator*(scale_t s, const se_core::Tuple2& t1) {
    return (se_core::Tuple2(t1)).operator*=(s);
}

#endif /* TUPLE2_H */
