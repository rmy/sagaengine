#include "PhFollowMouse.hpp"
#include "basic/area/NavMeshArea.hpp"
#include "sim/physics/PhysicsComponent.hpp"

using namespace se_basic;
using namespace se_client;
using namespace se_core;

namespace se_editor {

	void PhFollowMouse
	::calcNext(const PhysicsComponent& physics
			, const Pos& pos
			, Pos& nextPos
			, const Move& move
			, Move& nextMove
			) const {

		if(!nextPos.hasParent() || nextPos.parent() == nextPos.area())
			return;

		nextPos.world_ = pos.parent()->nextPos().world_;

		if(pos.parent()->nextPos().area() != nextPos.area()) {
			nextPos.setArea(const_cast<PosComponent&>(*pos.parent()->nextPos().area()));
		}
		nextPos.updateLocalViewPoint();
	}


	const PhFollowMouse physicsFollowMouse;
}
