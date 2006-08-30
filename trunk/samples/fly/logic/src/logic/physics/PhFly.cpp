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


#include "PhFly.hpp"
#include "sim/area/Area.hpp"

using namespace se_core;

namespace logic {

	void PhFly
	::calcNext(const Actor& actor
			, const Pos& pos
			, Pos& nextPos
			, const Move& move
			, Move& nextMove
			) const {
		// Calc nextMove
		Vector3 gravity(0, -4 * (COOR_RES / TIMESTEP_INTERVAL), 0);
		nextMove.velocity_.add( move.force_ );

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
		nextPos.localCoor().add(v);

		// Friction
		nextMove.velocity_.scale(0.8f);

		// Update world viewpoint
		nextPos.updateWorldViewPoint();

		// Entered new area?
		Area* old = nextPos.area();
		if(!nextPos.area()->isLegalCoor(nextPos.worldCoor())) {
			LogMsg("Out of bounds: "<< nextPos.area()->name() << ": " << nextPos.world_.toLog());
			Area* a = nextPos.area()->neighbour(nextPos.worldCoor());
			if(a) {
				// Change area, keep world viewpoint
				nextPos.setArea(*a, true);
				// Cannot use old index as hint for index in new area
				nextPos.setNoIndex();
			}
		}

		if(isBlocked(actor, pos, nextPos)) {
			// Revert back to original area if necessary
			if(nextPos.area() != old) {
				nextPos.setArea(*old, false);
			}
			nextPos.localCoor().x_ = pos.localCoor().x_;
			nextPos.localCoor().z_ = pos.localCoor().z_;
			nextPos.world_.coor_.x_ = pos.world_.coor_.x_;
			nextPos.world_.coor_.z_ = pos.world_.coor_.z_;
			// The previous index is valid again
			nextPos.index_ = pos.index_;
		}

		// Set the face direction to be equal to
		// movement direction
		nextMove.angularVelocity_.rotate( move.torque_ );
		nextPos.localFace().rotate(move.angularVelocity_);
		nextPos.localFace().normalize();

		// Friction
		nextMove.angularVelocity_.scale(0.9f);
		nextPos.localFace().roll_ = BrayT::scale(0.8f, nextPos.localFace().roll_);
		nextPos.localFace().pitch_ = BrayT::scale(0.85f, nextPos.localFace().pitch_);

		// Animation
		bray_t y = nextMove.angularVelocity_.yaw_;
		if(BrayT::isLeftwise(y)) {
			nextPos.anim(0).setMovementMode(1);
		}
		else {
			nextPos.anim(0).setMovementMode(2);
		}
		float w = BrayT::abs(y) / (float)(BRAY_RES) * 0.19;
		if(w < 0.02) w = 0.02;
		nextPos.anim(0).setSpeed(0);
		nextPos.anim(0).setStartPos(w);
		nextPos.anim(0).setWeight(w);

		static float maxW = 0;
		if(maxW < w) {
			maxW = w;
			LogMsg(maxW);
		}

		float sw = nextMove.velocity_.length() * .7;
		if(sw > 0.99999f) sw = 0.99999f;
		nextPos.anim(1).setMovementMode(1);
		nextPos.anim(1).setStartPos(sw);
		nextPos.anim(1).setWeight(sw);
		nextPos.anim(1).setSpeed(0);

		static float maxSw = 0;
		if(maxSw < sw) {
			maxSw = sw;
			LogMsg(maxSw);
		}

		nextPos.anim(2).setMovementMode(0);
		nextPos.anim(2).setSpeed(1);
		nextPos.anim(2).addStartPos(2 * (w + sw / 2) / (float)TIMESTEP_INTERVAL);

		// Some steering noise
		static Perlin p;
		float s = p.noise(pos.localCoor().x_ * 0.025f, pos.localCoor().y_ * 0.025f, pos.localCoor().y_ * 0.025f, 9, 9, 9, true, true, true) * 2;
		/*
		Euler3 noise(
					 BrayT::fromRad(.01 * (p.noise(pos.localCoor().x_ * 0.125, pos.localCoor().y_ * 0.125, 9, 9, true, true))),
					 BrayT::fromRad(.01 * (p.noise(pos.localCoor().y_ * 0.125, pos.localCoor().z_ * 0.125, 9, 9, true, true))),
					 BrayT::fromRad(.01 * (p.noise(pos.localCoor().z_ * 0.125, pos.localCoor().x_ * 0.125, 9, 9, true, true)))
				   );
		*/
		Euler3 noise(
					 BrayT::fromRad(.01f * (p.noise(pos.localCoor().x_, pos.localCoor().y_, 72, 72, true, true))),
					 BrayT::fromRad(.01f * (p.noise(pos.localCoor().y_, pos.localCoor().z_, 72, 72, true, true))),
					 BrayT::fromRad(.01f * (p.noise(pos.localCoor().z_, pos.localCoor().x_, 72, 72, true, true)))
				   );
		noise.scale(s);
		nextPos.localFace().add(noise);

		// local and world viewpoint must agree before leaving
		// physics
		nextPos.updateWorldViewPoint();
	}


	bool PhFly
	::isBlocked(const Actor& actor, const Pos& pos, const Pos& nextPos) const {
		short to = nextPos.terrainStyle();
		if(to == se_core::Pos::TS_VOID) {
			return true;
		}

		return false;
	}

}
