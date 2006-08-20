#include "PhTrackingCamera.hpp"
#include "basic/area/SimpleArea.hpp"

using namespace se_basic;
using namespace se_client;
using namespace se_core;

namespace logic {

	const PhTrackingCamera physicsCamera;	


	void PhTrackingCamera
	::calcNext(const Actor& actor
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
		Area* a = ClientSchema::player->nextPos().area();
		if(a != 0 && a != nextPos.area()) {
			nextPos.setArea(*a);
		}

		// Stay glued to the players spawn point 0
		const ViewPoint* sp = ClientSchema::player->spawnPoint(0);
		nextPos.local_.setViewPoint(*sp);
		
		nextPos.updateWorldViewPoint();
	}


	void PhTrackingCamera
	::affect(Actor& actor) const {
		Pos& nextPos = actor.nextPos();
		if(!nextPos.hasArea()) {
			// Camera has left all areas - which means
			// it should be destroyed
			actor.scheduleForDestruction();
		}
	}

}
