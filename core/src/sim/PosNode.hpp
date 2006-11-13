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


#ifndef sim_PosNode_hpp
#define sim_PosNode_hpp

#include "./SimObject.hpp"
#include "./SimComposite.hpp"
#include "./action/sim_action.hpp"
#include "./config/sim_config.hpp"
#include "./pos/Pos.hpp"
#include "./pos/PosComponent.hpp"
#include "./stat/MultiSimObject.hpp"
#include "./stat/sim_stat.hpp"
#include "./area/sim_area.hpp"
#include "util/vecmath/Point3.hpp"
#include "util/vecmath/ViewPoint.hpp"
#include "util/type/util_type.hpp"
#include "util/type/String.hpp"


namespace se_core {
	/** Parent class for entities that exists in and has a position in the game world.
	 *
	 * PosNode is the base class for all world entities, including
	 * the Area, Thing and Actor classes. It maintains information
	 * about the present local coordinate of a world entity, a
	 * reference to its parent, a list of its children, and methods
	 * for traversing the parents to calculate the world coordinates
	 * of the entity.
	 */
	class _SeCoreExport PosNode : public SimComposite {
	public:
		PosNode(enum SimObjectType type, const char* name);
		virtual ~PosNode();

		/** Cleanup this PosNode.
		 * Resets the parent, and removes it from child lists.
		 */
		virtual void cleanup() {
		}


		/** Returns true if object is or inherits from the given type
		 */
		virtual bool isType(enum SimObjectType type) const {
			return (type == got_POS_NODE);
		}

		/** Return a const reference to the current position of the thing.
		 *
		 * The current position should never be changed. Updates of the
		 * position for future steps should happen inside the nextPos().
		 * The nextPos() is copied into pos() at the next AI-step.
		 *
		 * The pos() object also contains information about the radius of the
		 * collision cylinder of the PosNode. This is used for the first step
		 * of the collision testing. The PosNodeCollide
		 * object registered with the PosNode may perform further collision
		 * testing.
		 */
		inline const Pos& pos() const {
			return posComponent_->pos();
		}

		/** Does the PosNode move during this simulation step?
		 */
		bool didMove() const {
			return posComponent_->didMove();
		}

		/**
		 * Reset the next coordinate.
		 * Sets the next coordinate to be the same as the present one.
		 */
		inline void resetFutureCoor() {
			posComponent_->resetFutureCoor();
		}

		inline void resetFutureXZCoor() {
			posComponent_->resetFutureXZCoor();
		}


		/** Returns false, signifying that a PosNode is not a mover.
		 *
		 * Only movers can collide into other things. As a PosNode never
		 * moves, it cannot collide into anything, and may be safely
		 * ignored in collision tests.
		 *
		 * An Actor can, however, collide into a PosNode.
		 */
		virtual bool isMover() const {
			return false;
		}

		/** Returns false, signifying that a PosNode is not a pusher.
		 */
		virtual bool isPusher() const {
			return false;
		}

		/** Can other PosNodes collide with this PosNode?
		 */
		bool isCollideable() const {
			return isCollideable_;
		}


		////// Methods that may be used by scripts - end //////

		/** Get the next coordinate.
		 *
		 * Get the precalculated Coor for where the Actor is at the beginning
		 * of the next (and the end of this) initiative.
		 */
		inline Pos& nextPos() {
			return posComponent_->nextPos();
		}

		inline const Pos& nextPos() const {
			return const_cast<PosComponent*>(posComponent_)->nextPos();
		}

		/** Flip from Pos() to nextPos()
		 *
		 * Make the next position the current position.
		 * This method is called by
		 * the Area the beginning of a new AI step.
		 */
		void flip() {
			return posComponent_->flip();
		}

		/**
		 * Add a PosNode as a child.
		 */
		//void addChildPosNode(PosNode& node);

		/**
		 * Add a PosNode from the child list.
		 */
		//void removeChildPosNode(PosNode& node);

		/**
		 * Get the list of child PosNodes.
		 */
		//MultiSimObject& childPosNodes() {
		//	return childPosNodes_;
		//}

		/**
		 * Remove the PosNode from the child list of its parent.
		 * You don't call this method directly. It is called automatically
		 * by the SagaEngine during the flip phase of a new simulation step.
		 */
		virtual void leaveCurrentParent() {
			posComponent_->leaveCurrentParent();
		}

		/**
		 * Move this PosNode from the child list of the old parent, to the child list of the new.
		 * You don't call this method directly. It is called automatically
		 * by the SagaEngine during the flip phase of a new simulation step.
		 */
		virtual bool changeParent() {
			return posComponent_->changeParent();
		}


		/** Remove the PosNode from its area.
		 *
		 * You don't call this method directly. It is called automatically
		 * by the SagaEngine during the flip phase of a new simulation step.
		 */
		virtual void leaveCurrentArea() {
			return posComponent_->leaveCurrentArea();
		}

		/** Move the PosNode from one area to another.
		 *
		 * You don't call this method directly. It is called automatically
		 * by the SagaEngine during the flip phase of a new simulation step.
		 */
		virtual bool changeArea() {
			return posComponent_->changeArea();
		}


		/** Set wether this thing can be collided with.
		 * 
		 * This is a hint to the engine that this should be included
		 * in the collison space.
		 */
		void setCollideable(bool isCollideable) {
			isCollideable_ = isCollideable;
		}

		////////////////////////////////////////

		/** Update the world viewpoint.
		 *
		 * Updates the world_ attribute in nextPos() make it harmonize with local_.
		 */
		void updateWorldViewPoint() {
			posComponent_->updateWorldViewPoint();
		}

		/** World coor interpolated between now and next.
		 *
		 * @param alpha Interpolation value. 0 gets the coordinate at pos(), 1 at nextPos()
		 * @param dest output value
		 */
		void worldCoor(scale_t alpha, Point3& dest) const {
			posComponent_->worldCoor(alpha, dest);
		}

		/** World viewpoint interpolated between now and next.
		 *
		 * @param alpha Interpolation value. 0 gets the coordinate at pos(), 1 at nextPos()
		 * @param dest output value
		 */
		void worldViewPoint(scale_t alpha, ViewPoint& dest) const {
			posComponent_->worldViewPoint(alpha, dest);
		}

		/** Set the list of spawn points associated with this PosNode.
		 *
		 * @param count the number of spawn points
		 * @param spawnPoints the list of spawn points
		 */
		void setSpawnPoints(int count, const ViewPoint* const* spawnPoints) {
			posComponent_->setSpawnPoints(count, spawnPoints);
		}

		/** Get the reference to a spawn point
		 *
		 * @param id the id of the sapwn points
		 */
		const ViewPoint* spawnPoint(short id) const {
			return posComponent_->spawnPoint(id);
		}

	protected:
		PosComponent* posComponent_;

		/** Position of the PosNode at the beginning of the current simulation step */
		//Pos position_;

		/** Position of the PosNode at the beginning of the next simulation step */
		//Pos nextPosition_;

		/** Flag which indicates that the PosNode moves between the current and the next simulation step. */
		//bool didMove_;

		/** Flag that indicated wether other PosNodes can collide with this PosNode.
		 * Used by the Area class to decide wether the PosNode should be inserted into a collision grid. 
		 */
		bool isCollideable_;

		/** The number of spawn points */
		//int spawnPointCount_;

		/** List of spawn points associated this this PosNode */
		//const ViewPoint* const* spawnPoints_;
	};

}

#endif
