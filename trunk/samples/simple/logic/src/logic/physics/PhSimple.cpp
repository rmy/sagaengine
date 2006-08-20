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


#include "PhSimple.hpp"
#include "basic/area/SimpleArea.hpp"

using namespace se_basic;
using namespace se_core;

namespace logic {

	void PhSimple
	::calcNext(const Actor& actor
			, const Pos& pos
			, Pos& nextPos
			, const Move& move
			, Move& nextMove
			) const {

		// Are physics meaningful at all?
		if(!pos.isKeyFramePath(nextPos)) {
			// If not skip it
			LogMsg("Did not find keyframe path for " << actor.name());
			return;
		}


		// Calc nextMove
		Vector3 gravity(0, -4 * (COOR_RES / TIMESTEP_INTERVAL), 0);
		nextMove.velocity_.add( move.force_ );
		nextMove.velocity_.add( gravity );
		nextMove.angularVelocity_.add( move.torque_ );

		// Calculate the change in the coordinate
		// that should affect the mover this tick
		Vector3 v(nextMove.velocity_);

		// Calculate the next position from the present
		// position and its change
		nextPos.localCoor().add(v);

		if(isBlocked(actor, pos, nextPos)) {
			nextPos.localCoor().x_ = pos.localCoor().x_;
			nextPos.localCoor().z_ = pos.localCoor().z_;
		}

		// Ground height at present position
		coor_t gh = nextPos.area()->groundHeight(nextPos.localCoor());
		// If below ground, then make grounded
		if(gh > nextPos.localCoor().y_) {
			nextPos.setGrounded(true);
		}
		// Clamp the character to the ground if grounded
		if(!nextPos.isGrounded()) {
			nextPos.localCoor().y_ = gh;
			nextMove.velocity_.y_ = 0;
		}

		// Set the face direction to be equal to
		// movement direction
		nextMove.angularVelocity_.rotate( move.torque_ );
		nextPos.localFace().rotate( move.angularVelocity_ );
		nextPos.localFace().normalize();

		// Friction
		nextMove.velocity_.scale(0.8);
		nextMove.angularVelocity_.scale(0.8);

		nextPos.updateWorldViewPoint();
	}


	bool PhSimple
	::isBlocked(const Actor& actor, const Pos& pos, const Pos& nextPos) const {
		short to = nextPos.terrainStyle();
		if(to == SimpleArea::TS_VOID) {
			return true;
		}

		return false;
	}

}
