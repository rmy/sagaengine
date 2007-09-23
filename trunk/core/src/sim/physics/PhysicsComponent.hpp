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
#include "Move.hpp"
#include "comp/Component.hpp"
#include "comp/node/NodeComponent.hpp"
#include "../area/sim_area.hpp"
#include "../custom/sim_custom.hpp"
#include "../stat/sim_stat.hpp"
#include "../thing/sim_thing.hpp"
#include "../pos/PosComponent.hpp"
#include "../react/ThingCollide.hpp"

namespace se_core {
	class _SeCoreExport PhysicsComponent : public NodeComponent {
	public:
		typedef ComponentPtr<PhysicsComponent, sct_PHYSICS> Ptr;

		static PhysicsComponent* get(Composite& composite) {
			PhysicsComponent* c = static_cast<PhysicsComponent*>(composite.component(se_core::sct_PHYSICS));
			return c;
		}

		static const PhysicsComponent* get(const Composite& composite) {
			const PhysicsComponent* c = static_cast<const PhysicsComponent*>(composite.component(se_core::sct_PHYSICS));
			return c;
		}

		static PhysicsComponent* get(Component& component) {
			PhysicsComponent* c = static_cast<PhysicsComponent*>(component.owner()->component(se_core::sct_PHYSICS));
			return c;
		}

		static const PhysicsComponent* get(const Component& component) {
			const PhysicsComponent* c = static_cast<const PhysicsComponent*>(component.owner()->component(se_core::sct_PHYSICS));
			return c;
		}



		/** Constructor.
		 */
		PhysicsComponent(Composite* owner, PosComponent* posComponent);
		PhysicsComponent(Composite* owner, const ComponentFactory* factory);
		~PhysicsComponent();

		bool calcNextCoor();

		void affect() {
			if(!affect_)
				return;
			affect_->affect(*this);
		}

		void pushPhysics(const char* name);
		void pushPhysics(const Physics *ph);
		void popPhysics();
		void clearPhysics();

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

		void flip();
		bool didMove() const {
			return didMove_;
		}

		//inline bool hasPhysics() const {
		//	return (currentPhysics_ >= 0 && physics_[currentPhysics_] != 0);
		//}

	private:
		inline bool hasDefaultPhysics() const {
			return (physics_[0] != 0);
		}

		inline const Physics& physics() {
			return *physics_[currentPhysics_];
		}
		inline const Physics& physics() const {
			return *physics_[currentPhysics_];
		}

		short currentPhysics_;
		static const short MAX_PHYSICS_STACK_SIZE = 4;
		const Physics* physics_[ MAX_PHYSICS_STACK_SIZE ];
		const Physics* affect_;
		friend class PhysicsAreaComponent;
		PosComponent* posComponent_;

		Move move_, nextMove_;

		bool didMove_;
	};
}

#endif
