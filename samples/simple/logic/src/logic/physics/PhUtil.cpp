#include "PhUtil.hpp"
#include "basic/area/NavMeshArea.hpp"
#include "util/math/all.hpp"
#include "util/vecmath/all.hpp"
#include "sim/react/CollisionAreaComponent.hpp"
#include "sim/react/AreaEdge.hpp"

using namespace se_basic;
using namespace se_core;

namespace logic {
	const Vector3 PhUtil
	::GRAVITY(0, -1.5f * COOR_STEP, 0);

	void PhUtil
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
		updateForces( physics, nextMove );

		// Friction
		applyFriction(nextMove);

		// Calc nextMove
		applyForces( pos, nextPos, move, nextMove);

		// Entered new area?
		nextPos.updateArea();

		// Check if blocked
		if(isBlocked(physics, pos, nextPos)) {
			// Revert back to original area if necessary
			blocked(physics, pos, nextPos, move, nextMove);
		}

		// Ground height at present position
		clampToGround(pos, nextPos, nextMove);
	}


	bool PhUtil
	::isBlocked(const PhysicsComponent& physics, const Pos& pos, Pos& nextPos) const {
		CollisionAreaComponent::Ptr cac(pos.area());
		Point3 areaCoor;
		nextPos.areaCoor(*pos.area(), areaCoor);
		Point2 dest, testPoint(areaCoor.x_, areaCoor.z_);
		coor_t dist = cac->areaEdge().distance(testPoint, dest);

		if(nextPos.radius() > dist) {
			//LogDetail(physics.owner()->name() << ": " << dist << " == " << dest.distance(testPoint));
			return true;
		}

		if(!nextPos.hasIndex()) {
			LogDetail("Doesn't have index: " << physics.owner()->name());
			return true;
		}

		/* Fails if y is below ground
		if(!nextPos.area()->nextPos().hasInside(nextPos.worldCoor())) {
			return true;
		}
		*/

		short to = nextPos.terrainStyle();
		if(to == NavMeshArea::TS_VOID) {
			LogDetail("Void");
			return true;
		}

		return false;
	}


	void PhUtil
	::blocked(const PhysicsComponent& physics
			, const Pos& pos
			, Pos& nextPos
			, const Move& move
			, Move& nextMove
			) const {

		CollisionAreaComponent::Ptr cac(pos.area());
		PosComponent::Ptr pPos(physics);
		Point3 areaCoor, nextAreaCoor;
		pos.areaCoor(*nextPos.area(), areaCoor);
		nextPos.areaCoor(*nextPos.area(), nextAreaCoor);
		Point2 dest, oldP(areaCoor.x_, areaCoor.z_), nextP(nextAreaCoor.x_, nextAreaCoor.z_);
		//coor_t dist = 
		cac->areaEdge().distance(nextP, dest);

		///
		Point3 p2(dest.x_, areaCoor.y_, dest.y_);
		//coor_t speed1 = nextMove.velocity_.length();

		if(cac->areaEdge().isGuilty(oldP, nextP)) {
			if(slide(nextMove, nextAreaCoor, p2, pPos->nextPos().radius(), 0.2f)) {
				updateForces(physics, nextMove);
				applyFriction(nextMove);
			}
			else {
				nextMove = move;
				rebound(move, nextMove, nextAreaCoor, p2);
				revertXZ(pos, nextPos);
				updateForces(physics, nextMove);
				applyFriction(nextMove);
			}
		}

		//coor_t speed2 = nextMove.velocity_.length();
		//AssertWarning(speed1 >= speed2, speed1 << " > " << speed2);

		///
		/*
		// Bounce in wall
		const NavMeshArea::Ptr area(pos.area());
		bray_t slideYaw = area->wallAngle(pos, nextPos.worldCoor());
		bray_t d = nextMove.velocity_.yaw();
		coor_t speed = CoorT::sqrt(nextMove.velocity_.xzLengthSquared());

		d = BrayT::sub(d, slideYaw);
		coor_t speedAfter = Trig::sinScale(speed, BrayT::abs(d));
		d = BrayT::sub(slideYaw, d);

		Vector3 v;
		v.setForward(speedAfter, d);
		v.y_ = nextMove.velocity_.y_;
	
		
		const PhysicsComponent::Ptr pPhysics(physics);
		// Don't get stuck in corners
		if(pPhysics->didMove()) {
			v.scale(nextMove.bounceMaintain_);
			v.y_ = nextMove.velocity_.y_;
		}
		nextMove.velocity_.set(v);

		revertXZ(pos, nextPos);
		*/
	}

	void PhUtil
	::rebound(const Move& move, Move& nextMove, const Point3& p1, const Point3& p2) const {
		const Vector3& v1 = nextMove.velocity_;

		//LogWarning(p1.distance(p2));
		// First, find the normalized vector n from the center of 
		// circle1 to the center of circle2
		Vector3 n;
		n.sub(p1, p2);
		n.y_ = 0;
		if(n.isZero())
			return;
		n.normalize(); 

		// Find the length of the component of each of the movement
		// vectors along n. 
		// a1 = v1 . n
		// a2 = v2 . n
		float a1 = v1.dot(n);

		// Calculate v1', the new movement vector of circle1
		Vector3 v1d;
		//scale_t bounceDecay = (1 - pPhysics->move().bounceMaintain_);
		//v1d.scale(-2 * a1 * (1 - bounceDecay * .5f) , n);
		v1d.scale(-2 * a1, n);
		nextMove.didBounce_ = true;
		v1d.y_ = 0;
		nextMove.addForce(v1d);
	}


	bool PhUtil
	::slide(Move& nextMove, const Point3& p1, const Point3& p2, coor_t rad, float threshold) const {
		// 
		const coor_double_t SLIDE_THRESHOLD_SQ = threshold * threshold;
		if(threshold >= 0 && nextMove.velocity_.lengthSquared() > SLIDE_THRESHOLD_SQ) {
			return false;
		}

		// 
		coor_t distanceSq = p1.xzDistanceSquared(p2);
		coor_t penetration = (rad - CoorT::sqrt(distanceSq));
		penetration *= 2;
		if(rad < 1)
			penetration /= rad;
		if(penetration < 0)
			penetration = 0;
		_away(nextMove, p1, p2, penetration * penetration + 2 * COOR_STEP);

		return true;
	}

	void PhUtil
	::_away(Move& nextMove, const Point3& p1, const Point3& p2, coor_t speed) const {
		Point3 aw;

		aw.sub(p2, p1);
		aw.y_ = 0;
		if(aw.isZero()) {
			aw.x_ = 1;
		}
		else {
			aw.normalize();
		}
		aw.scale(-speed);
		aw.y_ = 0;
		nextMove.addForce(aw);
	}

	void PhUtil
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

	void PhUtil
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


	void PhUtil
	::updateForces(const se_core::PhysicsComponent& physics, Move& nextMove) const {
		static const float MAX_SPEED = 128 * COOR_STEP;
		static const float MAX_SPEED_SQ = MAX_SPEED * MAX_SPEED;
		// Update forces
		nextMove.velocity_.add( nextMove.force_ );
		nextMove.angularVelocity_.rotate( nextMove.torque_ );

		if(nextMove.velocity_.xzLengthSquared() > MAX_SPEED_SQ) {
			Vector2 v(nextMove.velocity_.x_, nextMove.velocity_.z_);
			LogDetail(physics.owner()->name() << ": " << name() << " - " << ": " << v.length() << " - " << v.length() / COOR_STEP);
			v.normalize();
			v.scale( MAX_SPEED );
			nextMove.velocity_.set(v.x_, nextMove.velocity_.y_, v.y_);
		}
	}


	void PhUtil
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
			LogDetail("Scale velocity A: " << nextMove.velocity_.length() << " - " << nextMove.bounceMaintain_);
			nextMove.velocity_.scale(nextMove.bounceMaintain_);
			LogDetail("Scale velocity B: " << nextMove.velocity_.length());
		}

		// Linear friction
		// Scale
		coor_t vy = nextMove.velocity_.y_;
		nextMove.velocity_.scale(1 - nextMove.linearFrictionR_);
		nextMove.velocity_.y_ = vy;

		// Subtract
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


	void PhUtil
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


	void PhUtil
	::clampToGround(const Pos& pos
			, Pos& nextPos
			, Move& nextMove
			) const {
		// Ground height at present position
		NavMeshArea::Ptr nextArea(nextPos.area());
		if(nextArea.isNull())
			return;
		if(nextPos.index() < 0)
			return;
		coor_t gh = nextArea->groundHeight(nextPos.localCoor(), nextPos.index());
		// If below ground, then make grounded
		bool wasGrounded = nextPos.isGrounded();
		nextPos.setGrounded(gh >= nextPos.localCoor().y_);

		// Clamp the character to the ground if grounded
		if(nextPos.isGrounded()) {
			nextPos.worldCoor().y_ = gh;
			if(wasGrounded) {
				nextMove.velocity_.y_ = 0;
			}
			else {
				nextMove.velocity_.y_ = -nextMove.floorBounce_ * nextMove.velocity_.y_;
			}
		}

		nextPos.updateLocalViewPoint();
	}

}
