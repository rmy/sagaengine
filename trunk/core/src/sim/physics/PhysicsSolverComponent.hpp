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


#ifndef engine_physics_PhysicsSolverComponent_hpp
#define engine_physics_PhysicsSolverComponent_hpp

#include "Physics.hpp"
#include "../SimComponent.hpp"
#include "../area/CollisionGrid.hpp"
#include "../area/Area.hpp"

namespace se_core {
	class _SeCoreExport PhysicsSolverComponent : public SimComponent {
	public:
		/** Constructor.
		 */
		PhysicsSolverComponent(SimComposite* owner);
		~PhysicsSolverComponent();


		/**
		 * Sets the grid used to speed up the collisions detection.
		 * Often there an area is inactive much of the time, and a fine grained grid
		 * may take up quite a lot of memory. So the grid is passed around from
		 * one active area to the next.
		 */
		void setCollisionGrid(CollisionGrid* grid);

		CollisionGrid* collisionGrid() {
			return collisionGrid_; 
		}

		/**
		 * Resets collision grid.
		 * @see setCollisionGrid
		 */
		void resetCollisionGrid();
		void testActors2ThingsCollisions(Actor** movers, short moverCount);

		void addCollideable(Thing& posNode);
		void removeCollideable(Thing& posNode);

		void flipChildren();

		void setActive(bool state);

	private:
		CollisionGrid* collisionGrid_;

		/**
		 * List of movers this step. Updated by the coordinate precalcer and
		 * used by the collision detector.
		 */
		friend class PhysicsComponentManager;
		Actor** movers_;

		/** Number of movers presently in the movers_ array */
		short moverCount_;
	};
}

#endif
