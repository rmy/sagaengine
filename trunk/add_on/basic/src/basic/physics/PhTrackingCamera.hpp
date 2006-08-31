#ifndef logic_physics_PhTrackingCamera_hpp
#define logic_physics_PhTrackingCamera_hpp

#include "BasicPre.hpp"

namespace se_basic {

	class _SeBasicExport PhTrackingCamera : public se_core::Physics {
	public:
		PhTrackingCamera() : se_core::Physics("TrackingCamera") {}

		void calcNext(const se_core::Actor& actor
				, const se_core::Pos& pos
				, se_core::Pos& nextPos
				, const se_core::Move& move
				, se_core::Move& nextMove
				) const;

		void affect(se_core::Actor& actor) const;

	private:
	};


	extern _SeBasicExport const PhTrackingCamera physicsTrackingCamera;	
}

#endif
