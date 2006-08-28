#ifndef logic_physics_PhNavMesh_hpp
#define logic_physics_PhNavMesh_hpp

#include "LogicPre.hpp"

namespace logic {

	class _NavMeshLogicExport PhNavMesh : public se_core::Physics {
	public:
		PhNavMesh() : se_core::Physics("NavMesh") {}

		void calcNext(const se_core::Actor& actor
				, const se_core::Pos& pos
				, se_core::Pos& nextPos
				, const se_core::Move& move
				, se_core::Move& nextMove
				) const;

		bool isBlocked(const se_core::Actor& actor
				, const se_core::Pos& pos
				, const se_core::Pos& nextPos
				) const;

		void affect(se_core::Actor& actor) const {}

	private:
	};
}

#endif
