#include "PhCameraAbove.hpp"
#include "basic/area/NavMeshArea.hpp"
#include "sim/physics/PhysicsComponent.hpp"

using namespace se_basic;
using namespace se_client;
using namespace se_core;

namespace se_editor {

	void PhCameraAbove
	::calcNext(const PhysicsComponent& physics
			, const Pos& pos
			, Pos& nextPos
			, const Move& move
			, Move& nextMove
			) const {

		if(!nextPos.hasParent())
			return;

		nextPos.world_.coor_ = nextPos.parent()->nextPos().worldCoor();
		nextPos.world_.coor_.y_ += 48;
		//nextPos.world_.face_ = nextPos.parent()->nextPos().worldFace();
		nextPos.world_.face_.yaw_ = 0;
		nextPos.world_.face_.pitch_ = BrayT::DEG90;

		if(nextPos.parent()->nextPos().area() != nextPos.area()) {
			nextPos.setArea(*nextPos.parent()->nextPos().area());
		}
		nextPos.updateLocalViewPoint();
	}


	const PhCameraAbove physicsCameraAbove;
}
