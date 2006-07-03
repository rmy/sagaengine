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


#include "PhNavMesh.hpp"
#include "../area/NavMeshArea.hpp"

using namespace se_core;

namespace game {

	void PhNavMesh
	::calcNext(const Actor& actor
			, const Pos& pos
			, Pos& nextPos
			, Anim& nextAnim
			, Move& nextMove
			) const {

		const Move& move = actor.move();

		// Calc nextMove
		Force gravity(0, -4 * (COOR_RES / TIMESTEP_INTERVAL), 0);
		nextMove.velocity_.add( move.force_ );
		nextMove.velocity_.add( gravity );
		nextMove.angularVelocity_.add( move.torque_ );

		// Are physics meaningful at all?
		if(!pos.isKeyFramePath(nextPos)) {
			// If not skip it
			return;
		}

		// Calculate the change in the coordinate
		// that should affect the mover this tick
		Vector3 v(nextMove.velocity_);

		// Calculate the next position from the present
		// position and its change
		nextPos.add(v);


		// Calc navigation mesh triangle id
		nextPos.area()->updateIndex(nextPos);

		if(isBlocked(actor, pos, nextPos)) {
			nextPos.x_ = pos.x_;
			nextPos.z_ = pos.z_;
			nextPos.layer_ = pos.layer_;
		}

		// Ground height at present position
		coor_t gh = nextPos.area()->groundHeight(nextPos, nextPos.index());
		// If below ground, then make grounded
		if(gh > nextPos.y_) {
			nextPos.setLayer();
		}
		// Clamp the character to the ground if grounded
		if(!nextPos.hasOwnHeight()) {
			nextPos.y_ = gh;
			nextMove.velocity_.y_ = 0;
		}


		// Set the face direction to be equal to
		// movement direction
		//nextPos.face().add( nextMove.angularVelocity_ );
		nextMove.changeYaw(BrayT::add(actor.move().yaw_, actor.move().angularVelocity_.yaw_));
		nextPos.face().setEuler(actor.move().yaw_);
	}


	bool PhNavMesh
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

}
