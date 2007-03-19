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


#ifndef sim_PosComponent_hpp
#define sim_PosComponent_hpp

#include "../SimComponent.hpp"
#include "../SimComposite.hpp"
#include "../sim.hpp"
#include "Pos.hpp"
#include "../config/sim_config.hpp"
#include "util/type/util_type.hpp"
#include "util/type/String.hpp"


namespace se_core {
	/** Parent class for entities that exists in and has a position in the game world.
	 *
	 * PosComponent is the base class for all world entities, including
	 * the Area, Thing and Actor classes. It maintains information
	 * about the present local coordinate of a world entity, a
	 * reference to its parent, a list of its children, and methods
	 * for traversing the parents to calculate the world coordinates
	 * of the entity.
	 */
	class _SeCoreExport PosComponent : public SimComponent {
	public:
		typedef Ptr<PosComponent, sct_POS> Ptr;

		PosComponent(Composite* owner);
		virtual ~PosComponent();

		static PosComponent* get(Composite* composite) {
			if(!composite)
				return 0;
			PosComponent* c = static_cast<PosComponent*>(composite->component(se_core::sct_POS));
			return c;
		}

		static PosComponent* get(Composite& composite) {
			PosComponent* c = static_cast<PosComponent*>(composite.component(se_core::sct_POS));
			return c;
		}

		static const PosComponent* get(const Composite& composite) {
			const PosComponent* c = static_cast<const PosComponent*>(composite.component(se_core::sct_POS));
			return c;
		}

		static PosComponent* get(Component& component) {
			PosComponent* c = static_cast<PosComponent*>(component.owner()->component(se_core::sct_POS));
			return c;
		}

		static const PosComponent* get(const Component& component) {
			const PosComponent* c = static_cast<const PosComponent*>(component.owner()->component(se_core::sct_POS));
			return c;
		}

		/** Return a const reference to the current position of the thing.
		 *
		 * The current position should never be changed. Updates of the
		 * position for future steps should happen inside the nextPos().
		 * The nextPos() is copied into pos() at the next AI-step.
		 *
		 * The pos() object also contains information about the radius of the
		 * collision cylinder of the PosComponent. This is used for the first step
		 * of the collision testing. The PosComponentCollide
		 * object registered with the PosComponent may perform further collision
		 * testing.
		 */
		inline const Pos& pos() const {
			return position_;
		}

		/** Does the PosComponent move during this simulation step?
		 */
		bool didMove() const {
			return didMove_;
		}

		/**
		 * Reset the next coordinate.
		 * Sets the next coordinate to be the same as the present one.
		 */
		inline void resetFutureCoor() { didMove_ = false; nextPos().setPos(position_); }

		inline void resetFutureXZCoor() {
			// Revert back to original area if necessary
			nextPosition_.setXZ(position_);
		}


		/** Get the next coordinate.
		 *
		 * Get the precalculated Coor for where the Actor is at the beginning
		 * of the next (and the end of this) initiative.
		 */
		inline Pos& nextPos() {
			return nextPosition_;
		}

		inline const Pos& nextPos() const {
			return nextPosition_;
		}

		/** Flip from Pos() to nextPos()
		 *
		 * Make the next position the current position.
		 * This method is called by
		 * the Area the beginning of a new AI step.
		 */
		void flip();

		/**
		 * Remove the PosNode from the child list of its parent.
		 * You don't call this method directly. It is called automatically
		 * by the SagaEngine during the flip phase of a new simulation step.
		 */
		virtual void leaveCurrentParent();

		/**
		 * Move this PosNode from the child list of the old parent, to the child list of the new.
		 * You don't call this method directly. It is called automatically
		 * by the SagaEngine during the flip phase of a new simulation step.
		 */
		virtual bool changeParent();


		////////////////////////////////////////

		/** Update the world viewpoint.
		 *
		 * Updates the world_ attribute in nextPos() make it harmonize with local_.
		 */
		void updateWorldViewPoint();

		/** World coor interpolated between now and next.
		 *
		 * @param alpha Interpolation value. 0 gets the coordinate at pos(), 1 at nextPos()
		 * @param dest output value
		 */
		void worldCoor(scale_t alpha, Point3& dest) const;

		/** World viewpoint interpolated between now and next.
		 *
		 * @param alpha Interpolation value. 0 gets the coordinate at pos(), 1 at nextPos()
		 * @param dest output value
		 */
		void worldViewPoint(scale_t alpha, ViewPoint& dest) const;


	protected:
		virtual void cleanup();


		/** Position of the PosComponent at the beginning of the current simulation step */
		Pos position_;

		/** Position of the PosComponent at the beginning of the next simulation step */
		Pos nextPosition_;

		/** Flag which indicates that the PosComponent moves between the current and the next simulation step. */
		bool didMove_;

		/** Flag that indicated wether other PosNodes can collide with this PosNode.
		 * Used by the Area class to decide wether the PosNode should be inserted into a collision grid. 
		 */
		bool isCollideable_;
	};

}

#endif
