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
#ifndef util_vecmath_AxisAngle4_hpp
#define util_vecmath_AxisAngle4_hpp

#include "util_vecmath.hpp"
#include "Vector3.hpp"
#include "../math/CoorT.hpp"
#include "../math/BrayT.hpp"

namespace se_core {

	/**
	 * A 4 element axis angle represented by 
	 * x,y,z,angle components. An axis angle is a rotation of angle (radians) about
	 * the vector (x,y,z).
	 * @version specification 1.1, implementation $Revision: 1.4 $, $Date: 1999/10/06 02:52:46 $
	 * @author Kenji hiranabe
	 */
	class AxisAngle4 {
		/*
		 * I assumed that the length of axis vectors is not significant.
		 */
	public:
		/**
		 * the type for values
		 */
		typedef coor_t value_type;

		/**
		 * The x coordinate.
		 */
		coor_t x_;

		/**
		 * The y coordinate.
		 */
		coor_t y_;

		/**
		 * The z coordinate.
		 */
		coor_t z_;

		/**
		 * The angle.
		 */
		bray_t angle_;

		/**
		 * Constructs and initializes an AxisAngle4 from the specified x, y, z,
		 * and angle.
		 * @param xvalue the x coordinate
		 * @param yvalue the y coordinate
		 * @param zvalue the z coordinate
		 * @param anglevalue the angle.
		 */
		AxisAngle4(coor_t xvalue, coor_t yvalue, coor_t zvalue, bray_t anglevalue):
			x_(xvalue), y_(yvalue), z_(zvalue), angle_(anglevalue) { }

		/**
		 * Constructs an uninitializes AxisAngle4.
		 */
		AxisAngle4() { }

		/**
		 * Constructs and initializes an AxisAngle4f from the specified axis
		 * and angle.
		 *
		 * @param axis the axis
		 * @param ang the angle
		 * @since Java 3D 1.2
		 */
		AxisAngle4(const Vector3& axis, bray_t ang):
			x_(axis.x_), y_(axis.y_), z_(axis.z_), angle_(ang) { }

		AxisAngle4(const coor_t len, const Euler3& a1) {
			set(len, a1);
		}


		void setIdentity() {
			x_ = y_ = 0;
			z_ = -1;
			angle_ = 0;
		}

		/**
		 * Sets the value of this AxisAngle4f to the specified axis and
		 * angle.
		 * @param axis the axis
		 * @param ang the angle
		 * @since Java 3D 1.2
		 */
		void set(const Vector3& axis, bray_t ang) {
			x_ = axis.x_;
			y_ = axis.y_;
			z_ = axis.z_;
			angle_ = ang;
		}

		/**
		 * Sets the value of this axis angle to the specified x,y,z,angle.
		 * @param x the x coordinate
		 * @param y the y coordinate
		 * @param z the z coordinate
		 * @param angle the angle
		 */
		void set(coor_t x, coor_t y, coor_t z, bray_t angle) {
			this->x_ = x;
			this->y_ = y;
			this->z_ = z_;
			this->angle_ = angle;
		}

		/**
		 * Sets the value of this axis angle to the value of axis angle t1.
		 * @param t1 the axis angle to be copied
		 */
		void set(const AxisAngle4& a1) {
			x_ = a1.x_;
			y_ = a1.y_;
			z_ = a1.z_;
			angle_ = a1.angle_;
		}


		/**
		 * Sets the value of this axis-angle to the rotational equivalent of the
		 * passed quaternion.
		 * @param q1 the Quat4
		 */
		void set(const Quat4& q1);


		void set(const coor_t len, const bray_t yaw);
		void set(const coor_t len, const bray_t yaw, const bray_t pitch);
		void set(const coor_t len, const Euler3& a1);

		// helper method
	protected:
		void setFromQuat(coor_t x, coor_t y, coor_t z, coor_t w);

	public:
		/**
		 * Returns true if all of the data members of AxisAngle4 t1 are equal to the corresponding
		 * data members in this
		 * @param a1 the vector with which the comparison is made.
		 */
		bool equals(const AxisAngle4& a1) const {
			return x_ == a1.x_ && y_ == a1.y_ && z_ == a1.z_ && angle_ == a1.angle_;
		}

		/**
		 * Returns true if the L-infinite distance between this axis-angle and axis-angle t1 is
		 * less than or equal to the epsilon parameter, otherwise returns false. The L-infinite
		 * distance is equal to MAX[abs(x1-x2), abs(y1-y2), abs(z1-z2), abs(angle1-angle2)].
		 * @param a1 the axis-angle to be compared to this axis-angle
		 * @param epsilon the threshold value
		 */
		bool epsilonEquals(const AxisAngle4& a1, coor_t epsilon, bray_t epsilonAngle = 1) const {
			return (CoorT::abs(a1.x_ - x_) <= epsilon) &&
				(CoorT::abs(a1.y_ - y_) <= epsilon) &&
				(CoorT::abs(a1.z_ - z_) <= epsilon) &&
				(BrayT::abs(a1.angle_ - angle_) <= epsilonAngle);
		}

		// copy constructor and operator = is made by complier

		bool operator==(const AxisAngle4& m1) const {
			return equals(m1);
		}

		const char* toLog() const;
	};
	
}

#endif
