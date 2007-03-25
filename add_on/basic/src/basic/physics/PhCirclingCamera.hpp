#ifndef logic_physics_PhCirclingCamera_hpp
#define logic_physics_PhCirclingCamera_hpp

#include "BasicPre.hpp"
#include "sim/physics/Physics.hpp"

namespace se_basic {

	class _SeBasicExport PhCirclingCamera : public se_core::Physics {
	public:
		PhCirclingCamera() : se_core::Physics("CirclingCamera") {}

		void calcNext(const se_core::PhysicsComponent& physics
				, const se_core::Pos& pos
				, se_core::Pos& nextPos
				, const se_core::Move& move
				, se_core::Move& nextMove
				) const;

		void affect(se_core::PhysicsComponent& physics) const;

	private:
	};


	extern _SeBasicExport const PhCirclingCamera physicsCirclingCamera;	
}

#endif
