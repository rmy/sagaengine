#ifndef logic_physics_PhPushBack_hpp
#define logic_physics_PhPushBack_hpp

#include "LogicPre.hpp"

namespace logic {

	class _NavMeshLogicExport PhPushBack : public se_core::Physics {
	public:
		PhPushBack() : se_core::Physics("PushBack") {}

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

		void affect(se_core::Actor& actor) const;

		bool isStacker() const { return false; }

	private:
	};


	extern const PhPushBack physicsPushBack;
}

#endif
