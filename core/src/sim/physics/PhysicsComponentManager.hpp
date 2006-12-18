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


#ifndef PhysicsComponentManager_hpp
#define PhysicsComponentManager_hpp

#include "sim_physics.hpp"
#include "../sim.hpp"
#include "../SimComponentManager.hpp"
#include "../thing/Actor.hpp"


namespace se_core {

	/**
	 * Base class for functionality component managers.
	 */
	class _SeCoreExport PhysicsComponentManager : public SimComponentManager {
	public:
		/**
		 * Construct PhysicsComponentManager.
		 */
		PhysicsComponentManager();


		/**
		 * Destructor.
		 */
		virtual ~PhysicsComponentManager();


		void step(long when);

		void cleanupGame();

		static PhysicsComponentManager& singleton();

		//
		friend class PhysicsSolverComponent;
		void setSolverActive(PhysicsSolverComponent* s);
		void setSolverInactive(PhysicsSolverComponent* s);

	private:
		void flip(long when);
		void affect(long when);
		void performPhysics(long when);


		/**
		 * Size of movers_. Make sure it is large enough to hold
		 * all things that moves in the active area(s)!
		 */
		const static int MAX_MOVER_COUNT = MAX_GAME_OBJECTS;

		/**
		 * List of movers this step. Updated by the coordinate precalcer and
		 * used by the collision detector.
		 */
		PhysicsComponent** movers_;

		/** Number of movers presently in the movers_ array */
		short moverCount_;

		//
		static const int MAX_ACTIVE = 3 * (7 * 7 * 7);
		int activeSolverCount_;
		PhysicsSolverComponent** activeSolvers_;

	};

}

#endif
