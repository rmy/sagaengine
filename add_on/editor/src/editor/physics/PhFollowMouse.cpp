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

		if(!nextPos.hasParent())
			return;

		nextPos.world_ = nextPos.parent()->nextPos().world_;

		if(nextPos.parent()->nextPos().area() != nextPos.area()) {
			nextPos.setArea(*nextPos.parent()->nextPos().area());
		}
		nextPos.updateLocalViewPoint();
	}


	const PhFollowMouse physicsFollowMouse;
}
