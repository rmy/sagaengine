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


#include "PhMissile.hpp"
#include "../area/NavMeshArea.hpp"

using namespace se_core;

namespace game {

	void PhMissile
	::calcNext(const Actor& actor
			, const Pos& pos
			, Pos& nextPos
			, Anim& nextAnim
			, Move& nextMove
			) const {

		const Move& move = actor.move();

		// Are physics meaningful at all?
		if(!pos.isKeyFramePath(nextPos)) {
			// If not skip it
			return;
		}

		// Calculate the change in the coordinate
		// that should affect the mover this tick
		nextPos.add(nextMove.velocity_);


		// Calc navigation mesh triangle id
		nextPos.area()->updateIndex(nextPos);

		// If below ground, then destroy missile
		static const coor_t epsilon = 0.000001;
		coor_t gh = nextPos.area()->groundHeight(nextPos, nextPos.index());
		if(gh + epsilon < nextPos.y_) {
			// Cant destroy it before affect, so just leave area here
			nextPos.resetArea();
		}
		else if(isBlocked(actor, pos, nextPos)) {
			// Cant destroy it before affect, so just leave area here
			nextPos.resetArea();
		}
	}


	bool PhMissile
	::isBlocked(const Actor& actor, const Pos& pos, const Pos& nextPos) const {
		if(!nextPos.hasIndex()) {
			return true;
		}
		short to = nextPos.terrainStyle();
		if(to == NavMeshArea::TS_VOID) {
			return true;
		}

		return false;
	}


	void PhMissile
	::affect(Actor& actor) const {
		Pos& nextPos = actor.nextPos();
		if(!nextPos.hasArea()) {
			// Missile has left all areas - which means
			// it should be destroyed
			actor.scheduleForDestruction();
		}
	}


}
