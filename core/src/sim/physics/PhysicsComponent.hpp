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


#ifndef engine_physics_PhysicsComponent_hpp
#define engine_physics_PhysicsComponent_hpp

#include "Physics.hpp"
#include "../SimComponent.hpp"
#include "../SimNodeComponent.hpp"
#include "../custom/Move.hpp"
#include "../SimObject.hpp"
#include "../area/sim_area.hpp"
#include "../custom/sim_custom.hpp"
#include "../stat/sim_stat.hpp"
#include "../thing/sim_thing.hpp"

namespace se_core {
	class _SeCoreExport PhysicsComponent : public SimNodeComponent {
	public:
		/** Constructor.
		 */
		PhysicsComponent(Actor* owner);
		~PhysicsComponent();

		const char* name() { return "PhysicsSolver"; }
		bool calcNextCoor();

		void affect() {
			physics().affect(*owner());
		}

		void pushPhysics(const char* name);
		void pushPhysics(const Physics *ph);
		void popPhysics();

		inline const Move& move() const {
			return move_;
		}

		inline Move& nextMove() {
			return nextMove_;
		}

		void setDefaultPhysics(const Physics* ph) {
			physics_[0] = ph;
		}

		bool isMover() const;

	private:
		inline bool hasPhysics() const {
			return (physics_[0] != 0);
		}

		inline const Physics& physics() {
			return *physics_[currentPhysics_];
		}
		inline const Physics& physics() const {
			return *physics_[currentPhysics_];
		}

		short currentPhysics_;
		static const short MAX_PHYSICS_STACK_SIZE = 3;
		const Physics* physics_[ MAX_PHYSICS_STACK_SIZE ];

		Move move_, nextMove_;

		bool didMove_;
	};
}

#endif