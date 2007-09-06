#include "PhCameraAbove.hpp"
#include "basic/area/NavMeshArea.hpp"
#include "sim/physics/PhysicsComponent.hpp"
#include "../schema/EditorSchema.hpp"

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

		switch(EditorSchema::cameraPos) {
		case EditorSchema::cam_ABOVE_HIGH:
			{
				nextPos.world_.coor_ = nextPos.parent()->nextPos().worldCoor();
				nextPos.world_.coor_.y_ += 48;
				nextPos.world_.face_.yaw_ = 0;
				nextPos.world_.face_.pitch_ = BrayT::DEG90;
			}
			break;
		case EditorSchema::cam_ABOVE_MEDIUM:
			{
				nextPos.world_.coor_ = nextPos.parent()->nextPos().worldCoor();
				nextPos.world_.coor_.y_ += 12;
				nextPos.world_.face_.yaw_ = 0;
				nextPos.world_.face_.pitch_ = BrayT::DEG90;
			}
			break;
		case EditorSchema::cam_ABOVE_LOW:
			{
				nextPos.world_.coor_ = nextPos.parent()->nextPos().worldCoor();
				nextPos.world_.coor_.y_ += 8;
				nextPos.world_.face_.yaw_ = 0;
				nextPos.world_.face_.pitch_ = BrayT::DEG90;
			}
			break;
		case EditorSchema::cam_BEHIND:
			{
				nextPos.world_.coor_ = nextPos.parent()->nextPos().worldCoor();
				nextPos.world_.coor_.y_ += 2;
				nextPos.world_.coor_.z_ += 8;
				nextPos.world_.face_.yaw_ = 0;
				nextPos.world_.face_.pitch_ = 0;
			}
			break;
		case EditorSchema::cam_LEFT:
			{
				nextPos.world_.coor_ = nextPos.parent()->nextPos().worldCoor();
				nextPos.world_.coor_.y_ += 2;
				nextPos.world_.coor_.x_ += 8;
				nextPos.world_.face_.yaw_ = BrayT::DEG270;
				nextPos.world_.face_.pitch_ = 0;
			}
			break;

		default:
			LogFatal("Illegal camera pos");
		}

		if(nextPos.parent()->nextPos().area() != nextPos.area()) {
			nextPos.setArea(*nextPos.parent()->nextPos().area());
		}
		nextPos.updateLocalViewPoint();
	}


	const PhCameraAbove physicsCameraAbove;
}
