#include "PhTrackingCamera.hpp"
#include "sim/area/Area.hpp"

using namespace se_basic;
using namespace se_client;
using namespace se_core;

namespace se_basic {

	const PhTrackingCamera physicsTrackingCamera;	


	void PhTrackingCamera
	::calcNext(const PhysicsComponent& physics
			, const Pos& pos
			, Pos& nextPos
			, const Move& move
			, Move& nextMove
			) const {
	
		// Are physics meaningful at all?
		if(!pos.isKeyFramePath(nextPos)) {
			// If not skip it
			return;
		}

		// Always stay in the same area as the player
		PosComponent* a = ClientSchema::player->nextPos().area();
		if(a != 0 && a != nextPos.area()) {
			nextPos.setArea(*a);
		}

		// Stay glued to the players spawn point 0
		const ViewPoint* sp = SpawnComponent::get(*ClientSchema::player)->spawnPoint(0);
		nextPos.local_.setViewPoint(*sp);
		
		nextPos.updateWorldViewPoint();
	}


	void PhTrackingCamera
	::affect(PhysicsComponent& physics) const {
		if(!PosComponent::Ptr(physics)->nextPos().hasArea()) {
			// Camera has left all areas - which means
			// it should be destroyed
			physics.owner()->scheduleForDestruction();
		}
	}

}
