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


#ifndef engine_physics_Physics_hpp
#define engine_physics_Physics_hpp

#include "../SimObject.hpp"
#include "../area/sim_area.hpp"
#include "../custom/sim_custom.hpp"
#include "../stat/sim_stat.hpp"
#include "../thing/sim_thing.hpp"

namespace se_core {
	class Physics : public SimObject {
	public:
		Physics(const char* name);

		/**
		 * Calculate the next position of the actor. The pos and
		 * nextPos parameter values will usually be the ones
		 * returned by actor.pos() and actor.nextPos(). But
		 * they are explicit parameters to enable movement
		 * prediction.
		 *
		 * No state changes other than those that applies to
		 * nextPos should happen in calcNext, because the results
		 * of this method should be reversible on collision,
		 * and be usable for prediction.
		 */
		virtual void calcNext(const Actor& actor
							  , const Pos& pos
							  , Pos& nextPos
							  , Anim& nextAnim
							  , Move& nextMove
				) const {
			calcNext(actor, pos, nextPos);
		}

		virtual void calcNext(const Actor& actor
							  , const Pos& pos
							  , Pos& nextPos
				) const {};

		/**
		 * Check if the mover is blocked by terrain when it is moving
		 * from this position to the next position.
		 *
		 * No state changes should happen inside the the actor, pos
		 * or nextPos object should happen because of this. It should
		 * just return true if the mover is blocked, false if not.
		 *
		 * The collision handler will use this method, and reset nextPos
		 * itself it true is returned.
		 *
		 * More advanced collisions should be handled within the calcNext
		 * method.
		 */
		virtual bool isBlocked(const Actor& actor
							   , const Pos& pos
							   , const Pos& nextPos
							   ) const = 0;


		/**
		 * State changes to actor as a result of its position
		 * should happen inside affect. These can include health
		 * reductions as a result of standing on a firepit,
		 * etc. Changes that happen because of moving into a
		 * new terrain type should be handled by terrainMode
		 * switcher, though.
		 */
		virtual void affect(Actor& actor) const = 0;

		/**
		 *	Should return false if the physics state should
		 *	be popped before any is pushed on top of it.
		 */
		virtual bool isStacker() const { return true; }
	private:
	};
}

#endif
