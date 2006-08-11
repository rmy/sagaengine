#include "PhCamera.hpp"
#include "../area/SimpleArea.hpp"

using namespace se_core;

namespace game {

	const PhCamera physicsCamera;	


	void PhCamera
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

		const ViewPoint* sp = ClientSchema::player->spawnPoint(1);
		Area* a = ClientSchema::player->nextPos().area();
		if(a != 0 && a != nextPos.area()) {
			nextPos.setArea(*a);
		}
		nextPos.local_.setViewPoint(*sp);
	}


	bool PhCamera
	::isBlocked(const Actor& actor, const Pos& pos, const Pos& nextPos) const {
		return false;
	}

	void PhCamera
	::affect(Actor& actor) const {
		Pos& nextPos = actor.nextPos();
		if(!nextPos.hasArea()) {
			// Camera has left all areas - which means
			// it should be destroyed
			actor.scheduleForDestruction();
		}
	}
	
}
