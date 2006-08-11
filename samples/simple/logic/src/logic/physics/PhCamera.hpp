#ifndef logic_physics_PhCamera_hpp
#define logic_physics_PhCamera_hpp

#include "LogicPre.hpp"

namespace logic {

	class PhCamera : public se_core::Physics {
	public:
		PhCamera() : se_core::Physics("Camera") {}

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

	private:
	};


	extern const PhCamera physicsCamera;	
}

#endif
