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


#ifndef engine_react_ThingCollide_hpp
#define engine_react_ThingCollide_hpp

#include "sim_react.hpp"
#include "../thing/sim_thing.hpp"
#include "../SimObject.hpp"
#include "../physics/sim_physics.hpp"
#include "../pos/sim_pos.hpp"

namespace se_core {
	/** Base reaction class for actor vs thing collisions. */
	class _SeCoreExport ThingCollide : public SimObject {
	public:
		ThingCollide(const char* name);

		/**
		 * @return true if the collision blocks the movement of the collider.
		 */
		virtual bool collide(CollisionComponent& pusher
					 , const CollisionComponent& target) const = 0;

	protected:
		/**
		 * Returns true if the pusher caused the collision.
		 * Utility function that is meant to be used from collide
		 * method in subclasses.
		 */
		bool isGuilty(CollisionComponent& pusher
					 , const CollisionComponent& target) const;

	private:
		bool isGuilty(PosComponent& pusher
					 , const PosComponent& target) const;

	};
}

#endif
