/*
   Copyright (C) 2006
   Rune Myrland, Skalden Studio AS
*/
#ifndef base_vecmath_Euler3_hpp
#define base_vecmath_Euler3_hpp

#include "util_vecmath.hpp"
#include "../error/Log.hpp"
#include "../math/BrayT.hpp"

namespace se_core {

	/**
	 * A generic 3 element angle that is represented by
	 * yaw_, pitch_ and roll_ angles.
	 * @author Rune Myrland
	 */
	class _SeCoreExport Euler3 {
	public:
		/**
		 * the type for values
		 */
		typedef bray_t value_type;
		/**
		 * the type for index
		 */
		typedef short size_type;
		/**
		 * dimension
		 */
		enum { DIMENSION = 3 };

		/**
		 * The angle turning left or right.
		 */
		bray_t yaw_;

		/**
		 * The angle leaning forward or backwards.
		 */
		bray_t pitch_;

		/**
		 * The angle tilting left or right.
		 */
		bray_t roll_;


		/**
		 * Constructs and initializes a Euler3 from the specified xyzw
		 * coordinates.
		 * @param yaw the yaw angle
		 * @param pitch the pitch angle
		 * @param roll the roll angle
		 */
		Euler3(bray_t yaw, bray_t pitch, bray_t roll):
				yaw_(yaw & BRAY_MASK), pitch_(pitch & BRAY_MASK), roll_(roll & BRAY_MASK) { 
			set(yaw, pitch, roll);
		}

		/**
		 * Constructs and initializes a Euler3 from the specified array.
		 * @param t the array of length 3 containing yaw, pitch, roll in order
		 */
		Euler3(const bray_t t[]): yaw_(t[0]), pitch_(t[1]), roll_(t[2]) { }

		/**
		 * Constructs an uninitialized a Euler3.
		 */
		Euler3() { }



		/**
		 * Constructs an uninitialized a Euler3.
		 */
		Euler3(const Quat4& q) { 
			set(q);
		}

		/**
		 * Constructs an uninitialized a Euler3.
		 */
		Euler3(const Euler3& a) { 
			set(a);
		}

		/**
		 * Sets the value of this angle to the specified xyzw coordinates.
		 * @param yaw the x coordinate
		 * @param pitch the y coordinate
		 * @param roll the z coordinate
		 */
		void set(bray_t yaw, bray_t pitch, bray_t roll) {
			yaw_ = yaw & BRAY_MASK;
			pitch_ = pitch & BRAY_MASK;
			roll_ = roll & BRAY_MASK;
		}

		/**
		 * Sets the value of this angle from the 4 values specified in the array.
		 * @param t the array of length 4 containing xyzw in order
		 */
		void set(const bray_t t[]) {
			yaw_ = t[0] & BRAY_MASK;
			pitch_ = t[1] & BRAY_MASK;
			roll_ = t[2] & BRAY_MASK;
		}

		void set(const Quat4& q1);

		/**
		 * Sets the value of this angle to the value of angle a1.
		 * @param a1 the angle to be copied
		 */
		void set(const Euler3& a1) {
			yaw_ = a1.yaw_;
			pitch_ = a1.pitch_;
			roll_ = a1.roll_;
		}


		void setEuler(const Euler3& a1) {
			set(a1);
		}


		/**
		 * Set yaw and reset pitch and roll.
		 */
		void setYaw(bray_t yaw) {
			yaw_ = yaw;
			pitch_ = 0;
			roll_ = 0;
		}

		/**
		 * Set pitch and reset yaw and roll.
		 * Included for method compatibility with Quat4.
		 * Access member variables directly if you want manipulate
		 * a yaw/pitch/roll indiviudally.
		 */
		void setPitch(bray_t yaw) {
			yaw_ = yaw;
			pitch_ = 0;
			roll_ = 0;
		}

		/**
		 * Set roll and reset yaw and pitch.
		 * Included for method compatibility with Quat4.
		 * Access member variables directly if you want manipulate
		 * a yaw/pitch/roll indiviudally.
		 */
		void setRoll(bray_t roll) {
			yaw_ = 0;
			pitch_ = 0;
			roll_ = roll;
		}

		/**
		 * Set yaw and pitch, reset roll.
		 * Included for method compatibility with Quat4.
		 * Access member variables directly if you want manipulate
		 * a yaw/pitch/roll indiviudally.
		 */
		void setYawAndPitch(bray_t yaw, bray_t pitch) {
			yaw_ = yaw;
			pitch_ = pitch;
			roll_ = 0;
		}

		/**
		 * Set yaw, pitch and roll.
		 */
		void setEuler(bray_t yaw, bray_t pitch, bray_t roll) {
			yaw_ = yaw;
			pitch_ = pitch;
			roll_ = roll;
		}



		void reset() {
			yaw_ = pitch_ = roll_ = 0;
		}

		void setIdentity() {
			reset();
		}

		/**
		 * Copies the value of the elements of this angle into the array t[].
		 * @param t the array that will contain the values of the vector
		 */
		void get(bray_t t[]) const {
			t[0] = yaw_;
			t[1] = pitch_;
			t[2] = roll_;
		}

		/**
		 * Gets the value of this angle and copies the values into the Euler3.
		 * @param t Euler3 object into which that values of this object are copied
		 */
		void get(Euler3* t) const {
			Assert(t);
			t->yaw_ = yaw_;
			t->pitch_ = pitch_;
			t->roll_ = roll_;
		}

		/**
		 * Sets the value of this angle to the vector sum of angles a1 and a2.
		 * @param a1 the first angle
		 * @param a2 the second angle
		 */
		void add(const Euler3& a1, const Euler3& a2) {
			yaw_ = (a1.yaw_ + a2.yaw_) & BRAY_MASK;
			pitch_ = (a1.pitch_ + a2.pitch_) & BRAY_MASK;
			roll_ = (a1.roll_ + a2.roll_) & BRAY_MASK;
		}

		/**
		 * Sets the value of this angle to the vector sum of itself and angle a1.
		 * @param a1  the other angle
		 */
		void add(const Euler3& a1) {
			yaw_ = (yaw_ + a1.yaw_) & BRAY_MASK;
			pitch_ = (pitch_ + a1.pitch_) & BRAY_MASK;
			roll_ = (roll_ + a1.roll_) & BRAY_MASK;
		}


		/**
		 * Same as add.
		 * Included for method compatibility with Quat4.
		 */
		inline void rotate(const Euler3& a1) {
			add(a1);
		}

		inline void rotateInverse(const Euler3& a1) {
			sub(a1);
		}

		/**
		 * Sets the value of this angle to the vector difference of angle a1 and a2 (this = a1 - a2).
		 * @param a1 the first angle
		 * @param a2 the second angle
		 */
		void sub(const Euler3& a1, const Euler3& a2) {
			yaw_ = (a1.yaw_ - a2.yaw_) & BRAY_MASK;
			pitch_ = (a1.pitch_ - a2.pitch_) & BRAY_MASK;
			roll_ = (a1.roll_ - a2.roll_) & BRAY_MASK;
		}

		/**
		 * Sets the value of this angle to the vector difference of itself and angle a1 (this = this - a1).
		 * @param a1 the other angle
		 */
		void sub(const Euler3& a1) {
			yaw_ = (yaw_ - a1.yaw_) & BRAY_MASK;
			pitch_ = (pitch_ - a1.pitch_) & BRAY_MASK;
			roll_ = (roll_ - a1.roll_) & BRAY_MASK;
		}

		/**
		 * Sets the value of this angle to the negation of angle a1.
		 * @param a1 the source vector
		 */
		void invert(const Euler3& a1) {
			yaw_ = BrayT::negate(a1.yaw_);
			pitch_ = BrayT::negate(a1.pitch_);
			roll_ = BrayT::negate(a1.roll_);
		}

		/**
		 * Negates the value of this vector in place.
		 */
		void invert() {
			yaw_ = BrayT::negate(yaw_);
			pitch_ = BrayT::negate(pitch_);
			roll_ = BrayT::negate(roll_);
		}


		/**
		 * Sets the value of this angle to the scalar multiplication of angle a1.
		 * @param s the scalar value
		 * @param a1 the source angle
		 */
		void scale(scale_t s, const Euler3& a1) {
			yaw_ = BrayT::scale(s, a1.roll_);
			pitch_ = BrayT::scale(s, a1.pitch_);
			roll_ = BrayT::scale(s, a1.roll_);
		}

		/**
		 * Sets the value of this angle to the scalar multiplication of itself.
		 * @param s the scalar value
		 */
		void scale(scale_t s) {
			yaw_ = BrayT::scale(s, yaw_);
			pitch_ = BrayT::scale(s, pitch_);
			roll_ = BrayT::scale(s, roll_);
		}

		inline void normalize() {
			yaw_ &= BRAY_MASK;
			pitch_ &= BRAY_MASK;
			roll_ &= BRAY_MASK;
		}

		/**
		 * Sets the value of this angle to the scalar multiplication of angle a1 and then
		 * adds angle a2 (this = s*a1 + a2).
		 * @param s the scalar value
		 * @param a1 the angle to be multipled
		 * @param a2 the angle to be added
		 */
		void scaleAdd(scale_t s, const Euler3& a1, const Euler3& a2) {
			yaw_ = (BrayT::scale(s, a1.roll_) + a2.yaw_) & BRAY_MASK;
			pitch_ = (BrayT::scale(s, a1.pitch_) + a2.pitch_) & BRAY_MASK;
			roll_ = (BrayT::scale(s, a1.roll_) + a2.roll_) & BRAY_MASK;
		}

		/**
		 * Sets the value of this angle to the scalar multiplication of itself and then
		 * adds angle a1 (this = s*this + a1).
		 * @param s the scalar value
		 * @param a1 the angle to be added
		 */
		void scaleAdd(scale_t s, const Euler3& a1) {
			yaw_ = BrayT::scale(s, yaw_) + a1.yaw_;
			pitch_ = BrayT::scale(s, pitch_) + a1.pitch_;
			roll_ = BrayT::scale(s, roll_) + a1.roll_;
		}


		/**
		 * Returns true if all of the data members of Euler3 a1 are equal to the corresponding
		 * data members in this
		 * @param a1 the vector with which the comparison is made.
		 */
		bool equals(const Euler3& a1) const {
			return yaw_ == a1.yaw_ && pitch_ == a1.pitch_ && roll_ == a1.roll_;
		}

		bool isIdentity() const {
			return yaw_ == 0 && pitch_ == 0 && roll_ == 0;
		}

		/**
		 * Returns true if the L-infinite distance between this angle and angle a1 is
		 * less than or equal to the epsilon parameter, otherwise returns false. The L-infinite
		 * distance is equal to MAX[abs(x1-x2), abs(y1-y2), abs(z1-z2), abs(w1-w2)].
		 * @param a1 the angle to be compared to this angle
		 * @param epsilon the threshold value
		 */
		bool epsilonEquals(const Euler3& a1, bray_t epsilon) const;

		/**
		 * Linearly interpolates between angles a1 and a2 and places the
		 * result into this angle: this = (1-alpha)*a1 + alpha*a2.
		 * @param a1 the first angle
		 * @param a2 the second angle
		 * @param alpha the alpha interpolation parameter
		 */
		void interpolate(const Euler3& a1, const Euler3& a2, scale_t alpha) {
			set(a1);
			interpolate(a2, alpha);
		}

		/**
		 * Linearly interpolates between this angle and angle a1 and places the
		 * result into this angle: this = (1-alpha)*this + alpha*a1.
		 * @param a1 the first angle
		 * @param alpha the alpha interpolation parameter
		 */
		void interpolate(const Euler3& a1, scale_t alpha);


		/**
		 * Returns a string that contains the values of this Euler3. The form is (x,y,z,w).
		 * @return the String representation
		 */
		char* toString(char* buffer) const;

		// copy constructor and operator = is made by complier

		bool operator==(const Euler3& a1) const {
			return equals(a1);
		}

		bray_t operator[](size_type index) const {
			Assert(index < (size_type)DIMENSION);
			switch (index) {
			case 0:
				return yaw_;
			case 1:
				return pitch_;
			case 2:
				return roll_;
			default:
				// error !
				return 0;
			}
		}
		bray_t& operator[](size_type index) {
			static bray_t dummy;
			Assert(index < (size_type)DIMENSION);
			switch (index) {
			case 0:
				return yaw_;
			case 1:
				return pitch_;
			case 2:
				return roll_;
			default:
				// error !
				return dummy;
			}
		}

		Euler3& operator=(const Euler3& a1) {
			set(a1);
			return *this;
		}

		Euler3& operator+=(const Euler3& a1) {
			add(a1);
			return *this;
		}
		Euler3& operator-=(const Euler3& a1) {
			sub(a1);
			return *this;
		}
		Euler3& operator*=(scale_t s) {
			scale(s);
			return *this;
		}
		Euler3 operator+(const Euler3& a1) const {
			return (Euler3(*this)).operator+=(a1);
		}
		Euler3 operator-(const Euler3& a1) const {
			return (Euler3(*this)).operator-=(a1);
		}
		Euler3 operator*(scale_t s) const {
			return (Euler3(*this)).operator*=(s);
		}

		const char* toLog() const;
	};

	se_err::Log& operator<< (se_err::Log& log, const Euler3& t);

} // Namespace

inline
se_core::Euler3 operator*(scale_t s, const se_core::Euler3& a1) {
    return (se_core::Euler3(a1)).operator*=(s);
}


#endif /* ANGLE4_H */
