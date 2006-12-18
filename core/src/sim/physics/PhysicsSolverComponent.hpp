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
#include "../SimNodeComponent.hpp"
#include "../react/CollisionGrid.hpp"
#include "../react/sim_react.hpp"
#include "../area/Area.hpp"
#include "../pos/PosComponent.hpp"

namespace se_core {
	class _SeCoreExport PhysicsSolverComponent : public SimNodeComponent {
	public:
		/** Constructor.
		 */
		PhysicsSolverComponent(SimComposite* owner, CollisionAreaComponent* cac);
		~PhysicsSolverComponent();

		const char* name() { return "PhysicsSolver"; }


		int performChildPhysics(PhysicsComponent** movers);

		void flipChildren();
		void affectChildren();

		void setActive(bool state);

	private:
		/**
		 * List of movers this step. Updated by the coordinate precalcer and
		 * used by the collision detector.
		 */
		friend class PhysicsComponentManager;
		PhysicsComponent** movers_;

		CollisionAreaComponent* collisionAreaComponent_;

		/** Number of movers presently in the movers_ array */
		short moverCount_;
	};
}

#endif
