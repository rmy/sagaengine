#ifndef se_editor_physics_PhCameraAbove_hpp
#define se_editor_physics_PhCameraAbove_hpp

#include "LogicPre.hpp"
#include "sim/physics/Physics.hpp"

namespace se_editor {

	class _SeEditorExport PhCameraAbove : public se_core::Physics {
	public:
		PhCameraAbove() : se_core::Physics("CameraAbove") {}

		void calcNext(const se_core::PhysicsComponent& physics
				, const se_core::Pos& pos
				, se_core::Pos& nextPos
				, const se_core::Move& move
				, se_core::Move& nextMove
				) const;

		void affect(se_core::PhysicsComponent& physics) const {}

	private:
	};


	extern _SeEditorExport const PhCameraAbove physicsCameraAbove;	
}

#endif
