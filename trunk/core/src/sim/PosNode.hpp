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
#include "./action/sim_action.hpp"
#include "./config/sim_config.hpp"
#include "./stat/Coor.hpp"
#include "./stat/Anim.hpp"
#include "./stat/Pos.hpp"
#include "./stat/SpawnPoint.hpp"
#include "./stat/MultiSimObject.hpp"
#include "./stat/sim_stat.hpp"
#include "./area/sim_area.hpp"
#include "util/type/util_type.hpp"
#include "util/type/String.hpp"


namespace se_core {
	/** A static thing.
	 *
	 * PosNode is the base class for all world entities, including
	 * the Area, Thing and Actor classes. It maintains information
	 * about the present local coordinate of a world entity, a
	 * reference to its parent, a list of its children, and methods
	 * for traversing the parents to calculate the world coordinates
	 * of the entity.
	 */
	class PosNode : public SimObject {
	public:
		PosNode(enum SimObjectType type, const char* name);
		virtual ~PosNode();
		virtual void cleanup();


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
			return position_;
		}


		inline const Anim& anim() const {
			return anim_;
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

		/**
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
			return nextPosition_;
		}

		inline Anim& nextAnim() {
			return nextAnim_;
		}


		/** Flip from Pos() to nextPos()
		 *
		 * Make the next position the current position.
		 * This method is called by
		 * the Area the beginning of a new AI step.
		 */
		void flip();

		PosNode *parent() {
			return position_.parent();
		}

		virtual void leaveCurrentParent();
		virtual bool changeParent();


		Area *area() {
			return position_.area();
		}

		virtual void leaveCurrentArea();
		virtual bool changeArea();

		void addChildPosNode(PosNode& node);
		void removeChildPosNode(PosNode& node);


		/** Set wether this thing can be collided with. This
		 * is a hint to the engine that this should be included
		 * in the collison space.
		 */
		void setCollideable(bool isCollideable) {
			isCollideable_ = isCollideable;
		}

		////////////////////////////////////////

		MultiSimObject& childPosNodes() {
			return childPosNodes_;
		}

		/** World coor interpolated between now and next.
		 */
		void worldCoor(scale_t alpha, Coor& dest) const;
		void worldCoor(Coor& dest) const;
		void nextWorldCoor(Coor& dest) const;

		void worldViewPoint(scale_t alpha, ViewPoint& dest) const;
		void worldViewPoint(ViewPoint& dest) const;
		void nextWorldViewPoint(ViewPoint& dest) const;

		void childViewPoint(ViewPoint& dest, PosNode* stopAtParent = 0) const;
		void nextChildViewPoint(ViewPoint& dest, PosNode* stopAtParent = 0) const;
		void childCoor(Coor& dest, PosNode* stopAtParent = 0) const;
		void nextChildCoor(Coor& dest, PosNode* stopAtParent = 0) const;

		void setSpawnPoints(int count, const SpawnPoint* const* spawnPoints);
		const SpawnPoint* spawnPoint(short id) const;

	protected:
		/** Position and volume info for the thing. */
		Pos position_, nextPosition_;
		Anim anim_, nextAnim_;
		bool isCollideable_;
		MultiSimObject childPosNodes_;

		/** The number of spawn points */
		int spawnPointCount_;
		/** Spawn points */
		const SpawnPoint* const* spawnPoints_;
	};

}

#endif
