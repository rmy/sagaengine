/*
SagaEngine library
Copyright (c) 2002-2006 Skalden Studio AS

This software is provided 'as-is', without any express or implied 
warranty. In no event will the authors be held liable for any 
damages arising from the use of this software.

Permission is granted to distribute the library under the terms of the 
Q Public License version 1.0. Be sure to read and understand the license
before using the library. It should be included here, or you may read it
at http://www.trolltech.com/products/qt/licenses/licensing/qpl

The original version of this library can be located at:
http://www.sagaengine.com/

Rune Myrland
rune@skalden.com
*/


#include "PhDanglingCamera.hpp"
#include "../area/SimpleArea.hpp"

using namespace se_core;

namespace logic {

	void PhDanglingCamera
	::calcNext(const Actor& actor
			   , const Pos& pos
			   , Pos& nextPos
			   , const Move& move
			   , Move& nextMove
			) const {

		// If the camera is not inside an area, physics are meaningless
		if(!pos.hasArea()) {
			return;
		}

		// Player has already been update, because it is set as
		// the parent by the "T" option in the data/logic/start/start.txt
		const Pos& playerPos = ClientSchema::player->nextPos();

		// As this camera dangles behind the player, the player should
		// be inside an area as well
		if(!playerPos.hasArea()) {
			return;
		}

		// Keep the camera inside the same area as the player.
		// (This camera cannot collide, so it doesn't matter if the 
		// camera coordinate is outside area bounds.)
		Area* a = ClientSchema::player->nextPos().area();
		if(a != 0 && a != nextPos.area()) {
			nextPos.setArea(*a);
		}	

		// Get the distance to the player
		coor_t dist = pos.worldCoor().distance(playerPos.worldCoor());

		// If they are on the same position, move camera
		// to the players spawn coordinate 0
		if(dist == 0) {
			const ViewPoint* sp = ClientSchema::player->spawnPoint(0);
			nextPos.local_.setViewPoint(*sp);
			nextPos.updateWorldViewPoint();
			return;
		}

		// The default is that the camera stays in position
		nextPos.worldCoor().set(pos.worldCoor());

		// Unless it is more than TRAIL_DISTANCE away
		const coor_t TRAIL_DISTANCE = COOR_RES * 1;
		if(dist > TRAIL_DISTANCE) {
			// s is always between 0 and 1
			scale_t s = TRAIL_DISTANCE / dist;

			// Interpolate between current position and the players position
			nextPos.worldCoor().interpolate(playerPos.worldCoor(), .5 - s * s * .5);
		}

		// Log at the player
		nextPos.worldCoor().eulerTowards(playerPos.worldCoor(), nextPos.worldFace());

		// Make the local viewpoint reflect the new world viewpoint
		nextPos.updateLocalViewPoint();
	}


}
