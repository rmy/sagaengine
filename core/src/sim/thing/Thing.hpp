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


#ifndef Thing_hpp
#define Thing_hpp

#include "sim_thing.hpp"
#include "../SimObject.hpp"
#include "../PosNode.hpp"
#include "../custom/ThingData.hpp"
#include "../action/sim_action.hpp"
#include "../config/sim_config.hpp"
#include "../stat/Coor.hpp"
#include "../stat/Anim.hpp"
#include "../stat/Pos.hpp"
#include "../stat/MultiSimObject.hpp"
#include "../stat/sim_stat.hpp"
#include "../area/sim_area.hpp"
#include "util/type/util_type.hpp"
#include "util/type/String.hpp"


namespace se_core {
	/** A static thing.
	 *
	 * A Thing is a static object in the world. It may be collided into (if
	 * this is the behaviour wanted), but it does not have the ability to act
	 * on its own.
	 */
	class Thing : public PosNode, public ThingData {
	protected:
		Thing(enum SimObjectType type, const char* name);
	public:
		Thing(const char* name);
		virtual ~Thing();
		virtual void cleanup();


		/** Schedule the Thing for destruction.
		 *
		 * The destruction will be delayed one or two AI steps before it
		 * is actually destroyed, to make sure that any reference to
		 * and Action upon the Thing in the world has a chance to be resolved
		 * first.
		 */
		virtual void scheduleForDestruction();

		////// Methods that may be used by scripts - begin //////
		/** Returns true if object is or inherits from the given type
		 */
		virtual bool isType(enum SimObjectType type) const {
			return (type == got_THING);
		}

		bool didMove() const {
			return didMove_;
		}


		/** Returns pointer to the spawner of a thing, or null for no spawner.
		 *
		 * A Thing cannot (by definition) collide with an Actor that spawned
		 * it. (TODO: Can a Thing be spawned? Will not all spawned things have
		 * the ability to perform Action?)
		 */
		Actor* spawner() const { return spawner_; }

		/** Returns true if the thing is pickable.
		 *
		 * The Area classes can search after the nearest Thing that are pickable
		 * by an Actor. A Thing is only included in the search if it isPickable().
		 *
		 * @see setPickable
		 */
		bool isPickable() const { return isPickable_; }

		/** Returns false, signifying that a Thing is not a mover.
		 *
		 * Only movers can collide into other things. As a Thing never
		 * moves, it cannot collide into anything, and may be safely
		 * ignored in collision tests.
		 *
		 * An Actor can, however, collide into a Thing.
		 */
		virtual bool isMover() const { return false; }

		/** Returns false, signifying that a Thing is not a pusher.
		 */
		virtual bool isPusher() const { return false; }

		/**
		 */
		bool isCollideable() const { return isCollideable_; }


		////// Methods that may be used by scripts - end //////

		/** Set wether the thing is pickable.
		 *
		 * @see isPickable
		 */
		void setPickable(bool isPickable) { isPickable_ = isPickable; }

		/** Set wether this thing can be collided with. This
		 * is a hint to the engine that this should be included
		 * in the collison space.
		 */
		void setCollideable(bool isCollideable) { isCollideable_ = isCollideable; }

		/** Returns true if Thing is scheduled for destruction.
		 *
		 * @see scheduleForDestruction
		 */
		inline bool isDead() { return isDead_; }

		void setSpawner(Actor* spawner);

		////////////////////////////////////////

	protected:
		friend class ThingManager;
		ThingFactory* factory_;

		/**
		 * The actor who spawned this thing.
		 * A bow shooter will for example spawn arrows when shooting. This
		 * variable is useful for making the shooter immune to shoots from
		 * his own arrows.
		 *
		 * This variable also makes it possible to maintain a spawn counter
		 * which indicates how many *living* children an actor has spawned.
		 * This enables monster spawner's to create new children whenever
		 * they fall below a certain count.
		 */
		Actor* spawner_;

		bool isPickable_;
		bool isCollideable_;
		bool isDead_;
		bool didMove_;

	public:
		DebugExec(static int refCount);
	};

}

#endif
