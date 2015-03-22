#ifndef logic_physics_PhSimple_hpp
#define logic_physics_PhSimple_hpp

#include "LogicPre.hpp"
#include "PhUtil.hpp"

namespace logic {

	class _SimpleLogicExport PhSimple : public PhUtil {
	public:
		PhSimple() : PhUtil("Simple") {}
		PhSimple(const char* name) : PhUtil(name) {}

		void calcNext(const se_core::PhysicsComponent& physics
				, const se_core::Pos& pos
				, se_core::Pos& nextPos
				, const se_core::Move& move
				, se_core::Move& nextMove
				) const;

		void affect(se_core::PhysicsComponent& physics) const;

		bool isStacker() const { return false; }

	private:
	};


	extern _SimpleLogicExport const PhSimple physicsSimple;
}

#endif
