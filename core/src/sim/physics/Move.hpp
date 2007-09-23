/*
SagaEngine library
Copyright (c) 2002-2006 Skalden Studio AS

This software is provided 'as-is', without any express or implied 
warranty. In no event will the authors be held liable for any 
damages arising from the use of this software.

Permission is granted to distribute the library under the terms of the 
Q Public License version 1.0. Be sure to read and understand the license
before using the library. It should be included here, or you may read it
at http://www.trolltech.com/products/qt/licenses/licensing/qpl

The original version of this library can be located at:
http://www.sagaengine.com/

Rune Myrland
rune@skalden.com
*/


#ifndef sim_custom_Move_hpp
#define sim_custom_Move_hpp

#include "util/config/util_config.hpp"

#ifdef SE_OWN_MOVE

#include "config/CustomMove.hpp"

#else

#include "../stat/sim_stat.hpp"
#include "util/vecmath/Quat4.hpp"
#include "util/vecmath/Euler3.hpp"
#include "util/vecmath/Point3.hpp"
#include "util/vecmath/Vector3.hpp"
#include "util/vecmath/ViewPoint.hpp"


namespace se_core {

	class _SeCoreExport Move {
	public:
		//
		Vector3 velocity_; // Velocity - the speed and direction at which the position changes
		Euler3 angularVelocity_; // Angular Velocity - the rate at which the orientation changes

		// Usually stable values
		scale_t mass_; // The mass of the body
		Point3 centerOfMass_; // The pivot point of the center of mass - relative to the position
		//Matrix3 inertia_; // How the mass is distributed around the center of mass

		// The sum of forces working on this body.
		Vector3 force_; // Forces applied (acceleration)
		Euler3 torque_; // Torque - forces changing rotation

		scale_t linearFriction_;
		scale_t linearFrictionR_;
		scale_t angularFriction_;
		bool didBounce_;
		scale_t bounceMaintain_;
		scale_t floorBounce_;
		int affectCode_;

		// The physics object is free to interpret these values as they like
		struct WorkVars {
			coor_t speed_;
			ViewPoint vp_;
		} work_;

	public:
		Move();
		void setMove(const Move& original);

		coor_t workSpeed() const { return work_.speed_; }
		bray_t workYaw() const { return work_.vp_.face_.yaw_; }

		/**
		 * Get speed in the xz-plane.
		 *
		 * @return The length of the speed vector in the xz-plane (coor_t units per step)
		 */
		const inline Vector3& velocity() const { return velocity_; }

		/**
		 * Set speed to zero.
		 */
		inline void resetSpeed() { work_.speed_ = 0; velocity_.reset(); }

		void changeYaw(bray_t yaw);
		void changeSpeed(coor_t speed);

		/**
		 * Change movement direction and speed in the xz-plane.
		 *
		 * @param direction Direction as braybrookians in xz-plane (a circle
		 *          is 256 braybrookians).
		 * @param speed The length of the new speed vector, coor_t units per step.
		 */
		void changeMovement(bray_t yaw, coor_t speed);

		/**
		 * Is this Pos trying to move this step?
		 *
		 * @return true if is trying to move, false if not.
		 */
		inline bool isMoving() const { return (velocity_.isZero() || !force_.isZero()); }

		/**
		 * Flick to the next step.
		 * Flicks the values that are reserved for use next step, so that they are used
		 * now. Performed by the SimEngine just before it precalculates coordinates
		 * for the next step.
		 */
		void resetForces();


		/**
		 * Get the sum of the forces that are working on this Pos this step.
		 *
		 * @return The sum of the forces presently working on this Pos.
		 */
		const Vector3& resultantForce() const { return force_; }

		/**
		 * Resets the forces that will work on this Pos the next step.
		 */
		void clearForces() { force_.reset(); }

		/**
		 * Adds a force that will work on this Pos the next step.
		 *
		 * @param force The force to add
		 */
		void addForce(const Vector3& force) { 
			force_.add(force); 
			Assert(!force_.isNan());
		}
		void addForce(scale_t s, const Vector3& force) { force_.scaleAdd(s, force, force_); }

		/**
		 * Subtracs a force that will work on this Pos the next step.
		 *
		 * @param force The force to subtract
		 */
		void subtractForce(const Vector3& force) { force_.sub(force); }

	};



}
#endif // SE_OWN_MOVE

#endif
