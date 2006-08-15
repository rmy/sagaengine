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

#ifndef logic_custom_Move_hpp
#define logic_custom_Move_hpp



#include "util/type/util_type.hpp"
#include "sim/stat/sim_stat.hpp"
#include "util/vecmath/Quat4.hpp"
#include "util/vecmath/Euler3.hpp"
#include "util/vecmath/Point3.hpp"
#include "util/vecmath/Vector3.hpp"


namespace se_core {

	class Move {
	public:
		//
		Vector3 velocity_; // Velocity - the speed and direction at which the position changes
		// The sum of forces working on this body.
		Vector3 force_; // Forces applied (acceleration)

		#ifdef SE_QUAT
		Quat4 angularVelocity_; // Angular Velocity - the rate at which the orientation changes
		Quat4 torque_; // Torque - forces changing rotation
		#else
		Euler3 angularVelocity_; // Angular Velocity - the rate at which the orientation changes
		Euler3 torque_; // Torque - forces changing rotation
		#endif

		// Usually stable values
		scale_t mass_; // The mass of the body
		Point3 centerOfMass_; // The pivot point of the center of mass - relative to the position
		//Matrix3 inertia_; // How the mass is distributed around the center of mass


	public:
		Move();
		void setMove(const Move& original);

		/**
		 * Get speed in the xz-plane.
		 *
		 * @return The length of the speed vector in the xz-plane (coor_t units per step)
		 */
		const inline Vector3& velocity() const { return velocity_; }

		/**
		 * Set speed to zero.
		 */
		inline void resetSpeed() { velocity_.reset(); }

		/**
		 * Is this Pos trying to move this step?
		 *
		 * @return true if is trying to move, false if not.
		 */
		inline bool isMoving() const { return (!velocity_.isZero() || !force_.isZero()); }

		/**
		 * Gets the pushing force of this Position.
		 * The pushing force presently consists of an x- and z-components
		 * of the present speed.
		 *
		 * @param dest The force to store the speed into.
		 * @return Refrecence to dest
		 */
		Vector3& pushForce(Vector3& dest) const;

		/**
		 * Flick to the next step.
		 * Flicks the values that are reserved for use next step, so that they are used
		 * now. Performed by the SimEngine just before it precalculates coordinates
		 * for the next step.
		 */
		void flick();


		/**
		 * Get the sum of the forces that are working on this mover this step.
		 *
		 * @return The sum of the forces presently working on this Pos.
		 */
		const Vector3& resultantForce() const { return force_; }

		/**
		 * Resets the forces that will work on this mover the next step.
		 */
		void clearForces() { force_.reset(); }

		/**
		 * Adds a force that will work on this mover the next step.
		 *
		 * @param force The force to add
		 */
		void addForce(const Vector3& force) { force_.add(force); }

		/**
		 * Subtracs a force that will work on this mover the next step.
		 *
		 * @param force The force to subtract
		 */
		void subtractForce(const Vector3& force) { force_.sub(force); }

		/**
		 * Calculate and store the next coordinate using the information
		 * in the original Pos as a starting point. If the Pos has a layer,
		 * the y coordinate will be updated to the height of the the xz coordinate
		 * for this layer in the map.
		 *
		 * @param original The object containing the original position, speed, force modiefiers, etc.
		 * @param area The area involving the heights.
		 */
		void calcNext(const Pos& original, const Move& move);

	};



}

#endif
