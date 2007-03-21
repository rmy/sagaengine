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


#include "PhDefault.hpp"
#include "../area/Area.hpp"
#include "../stat/all.hpp"
#include "util/math/all.hpp"


namespace se_core {
	const PhDefault physicsDefault;

	const Vector3 PhDefault
	::GRAVITY(0, -1.25f * COOR_STEP, 0);

	void PhDefault
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

		// Blend animations
		blendAnims(pos, nextPos);

		// Update forces
		updateForces( nextMove );

		// Friction
		applyFriction(nextMove);

		// Calc nextMove
		applyForces( pos, nextPos, move, nextMove);

		// Entered new area?
		PosComponent* old = nextPos.area();
		nextPos.updateArea();

		// Check if blocked
		if(isBlocked(physics, pos, nextPos)) {
			// Revert back to original area if necessary
			blocked(physics, pos, nextPos, move, nextMove);
		}

		// Ground height at present position
		clampToGround(pos, nextPos, nextMove);
	}


	bool PhDefault
	::isBlocked(const PhysicsComponent& physics, const Pos& pos, Pos& nextPos) const {
		if(!nextPos.hasIndex()) {
			return true;
		}

		/* Fails if y is below ground
		if(!nextPos.area()->nextPos().hasInside(nextPos.worldCoor())) {
			return true;
		}
		*/

		short to = nextPos.terrainStyle();
		if(to == 0) {
			return true;
		}

		return false;
	}


	void PhDefault
	::blocked(const PhysicsComponent& physics
			, const Pos& pos
			, Pos& nextPos
			, const Move& move
			, Move& nextMove
			) const {
		/*
		// Bounce in wall
		const NavMeshArea* area = static_cast<const NavMeshArea*>(pos.area()->owner());
		bray_t slideYaw = area->wallAngle(pos, nextPos.worldCoor());
		bray_t d = nextMove.velocity_.yaw();
		coor_t speed = nextMove.velocity_.length();

		d = BrayT::sub(d, slideYaw);
		coor_t speedAfter = Trig::sinScale(speed, BrayT::abs(d));
		d = BrayT::sub(slideYaw, d);

		Vector3 v, accel;
		v.setForward(speedAfter, d);
		accel.sub(v, nextMove.velocity_);

		// Don't get stuck in corners
		if(physics.didMove()) {
			v.scale(nextMove.bounceMaintain_);
		}
		nextMove.velocity_.set(v);
		*/

		revertXZ(pos, nextPos);
	}


	void PhDefault
	::blendAnims(const Pos& pos
			, Pos& nextPos
			) const {

		// Blend animations
		nextPos.anim(2).updatePos();
		nextPos.anim(3).updatePos();
		scale_t w = nextPos.anim(2).weight();
		if(w != 1) {
			w += 4 * SCALE_STEP;
			if(w > 1) w = 1;
			nextPos.anim(2).setWeight(w);
			nextPos.anim(3).setWeight(1 - w);
		}

		// Blend animations
		nextPos.anim(0).updatePos();
		nextPos.anim(1).updatePos();
		w = nextPos.anim(0).weight();
		if(w != 1) {
			w += 4 * SCALE_STEP;
			if(w > 1) w = 1;
			nextPos.anim(0).setWeight(w);
			nextPos.anim(1).setWeight(1 - w);
		}

	}

	void PhDefault
	::revertXZ(const Pos& pos
			, Pos& nextPos
			) const {
		PosComponent* old = const_cast<PosComponent*>(pos.area());
		if(nextPos.area() != old) {
			nextPos.setArea(*old, false);
		}
		// Revert coordinates in xz plane. (Can still fall.)
		nextPos.localCoor().x_ = pos.localCoor().x_;
		nextPos.localCoor().z_ = pos.localCoor().z_;
		nextPos.worldCoor().x_ = pos.worldCoor().x_;
		nextPos.worldCoor().z_ = pos.worldCoor().z_;
		// The previous index is valid again
		nextPos.index_ = pos.index_;
	}


	void PhDefault
	::updateForces(Move& nextMove) const {
		// Update forces
		nextMove.velocity_.add( nextMove.force_ );
		nextMove.angularVelocity_.rotate( nextMove.torque_ );
	}


	void PhDefault
	::applyFriction(Move& nextMove) const {
		// Apply friction
		nextMove.angularVelocity_.scale(1 - nextMove.angularFriction_);

		/* Friction increasion proportionally with speed.
		nextMove.velocity_.scale(nextMove.friction_);

		const coor_double_t threshold = CoorT::pow2(nextMove.threshold_);
		if(nextMove.velocity_.xzLengthSquared() < threshold) {
			nextMove.velocity_.x_ = 0;
			nextMove.velocity_.z_ = 0;
		}
		*/

		if(nextMove.didBounce_) {
			nextMove.velocity_.scale(nextMove.bounceMaintain_);
		}

		// Linear friction
		if(!nextMove.velocity_.isZero() && nextMove.linearFriction_ != 0) {
			Vector3 friction(nextMove.velocity_);
			friction.y_ = 0;
			if(!friction.isZero()) {
				friction.normalize();
				friction.scale(-nextMove.linearFriction_);
			}
			if(friction.xzLengthSquared() < nextMove.velocity().xzLengthSquared()) {
				nextMove.velocity_.add(friction);
			}
			else {
				nextMove.velocity_.x_ = 0;
				nextMove.velocity_.z_ = 0;
			}
		}
	}


	void PhDefault
	::applyForces(const Pos& pos
			, Pos& nextPos
			, const Move& move
			, Move& nextMove
			) const {
		// Apply forces
		nextPos.localCoor().add(nextMove.velocity_);
		nextPos.localFace().rotate( nextMove.angularVelocity_ );

		nextPos.updateWorldViewPoint();
	}


	void PhDefault
	::clampToGround(const Pos& pos
			, Pos& nextPos
			, Move& nextMove
			) const {
		// Ground height at present position
		Area* nextArea = static_cast<Area*>(nextPos.area()->owner());
		coor_t gh = nextArea->groundHeight(nextPos.localCoor(), nextPos.index());
		// If below ground, then make grounded
		nextPos.setGrounded(gh >= nextPos.localCoor().y_);

		// Clamp the character to the ground if grounded
		if(nextPos.isGrounded()) {
			nextPos.worldCoor().y_ = gh;
			nextMove.velocity_.y_ = 0;
		}

		nextPos.updateLocalViewPoint();
	}


}
