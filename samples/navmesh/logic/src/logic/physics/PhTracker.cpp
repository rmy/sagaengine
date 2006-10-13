#include "PhTracker.hpp"
#include "basic/area/NavMeshArea.hpp"

using namespace se_basic;
using namespace se_core;

namespace logic {

	void PhTracker
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

		// Calc nextMove
		Vector3 gravity(0, -3 * (COOR_RES / TIMESTEP_INTERVAL), 0);
		//nextMove.velocity_.setForward( nextMove.speed_, nextPos.worldFace().yaw_ + BrayT::DEG180 );
		//nextMove.velocity_.add( move.force_ );
		nextMove.velocity_.add( gravity );

		bray_t diffNow = BrayT::sub(move.workYaw(), nextPos.worldFace().yaw_);
		bray_t absDiff = BrayT::abs(diffNow);
		bray_t diffNext = BrayT::sub(diffNow, move.angularVelocity_.yaw_);

		const bray_t ROT_SPEED = (BRAY_RES);

		nextMove.torque_.yaw_ = 0;
		if(BrayT::isLeftwise(diffNow) 
		   && (!BrayT::isLeftwise(move.angularVelocity_.yaw_) 
			   || BrayT::abs(move.angularVelocity_.yaw_) < BRAY_RES * 4)) {
			nextMove.torque_.yaw_ = -ROT_SPEED;
		}

		if(BrayT::isRightwise(diffNow) 
		   && (!BrayT::isRightwise(move.angularVelocity_.yaw_) 
			   || BrayT::abs(move.angularVelocity_.yaw_) < BRAY_RES * 4)) {
			nextMove.torque_.yaw_ = ROT_SPEED;
		}

		nextMove.angularVelocity_.rotate( nextMove.torque_ );
		if(BrayT::abs(move.angularVelocity_.yaw_) * 4 > absDiff) {
			// Break rotation
			//	nextMove.angularVelocity_.scale((1.0f / 32.0f * BRAY_RES) * absDiff);
			nextMove.angularVelocity_.scale(1.0 - 1.0 / (1.0 * absDiff));
		}

		//if(BrayT::isLeftwise(diffNow) != BrayT::isLeftwise(diffNext)) {
		//	nextMove.angularVelocity_.yaw_ = 0;
		//	nextPos.worldFace().yaw_ = nextMove.yaw_;
		//}
		//else {
		nextPos.worldFace().rotate( nextMove.angularVelocity_ );
		if(diffNow < BRAY_RES) {
			nextMove.angularVelocity_.yaw_ = 0;
			nextPos.worldFace().yaw_ = nextMove.workYaw();
		}
		//}


		//nextMove.velocity_.setForward( nextMove.speed_, nextPos.worldFace().yaw_ + BrayT::DEG180 );



		// Friction
		nextMove.angularVelocity_.scale(0.6);
		nextMove.velocity_.scale(0.8);

		// Calculate the change in the coordinate
		// that should affect the mover this tick
		//Vector3 v(nextMove.velocity_);

		// Calculate the next position from the present
		// position and its change
		//nextPos.localCoor().add(v);



		nextPos.updateWorldViewPoint();

		// Entered new area?
		Area* old = nextPos.area();
		if(!nextPos.area()->isLegalCoor(nextPos.worldCoor())) {
			//LogMsg("Out of bounds: "<< nextPos.area()->name() << ": " << nextPos.world_.toLog());
			Area* a = nextPos.area()->neighbour(nextPos.worldCoor());
			if(a) {
				// Change area, keep world viewpoint
				nextPos.setArea(*a, true);
				// Cannot use old index as hint for index in new area
				nextPos.setNoIndex();
			}
		}

		// Calc navigation mesh triangle id
		short newIndex = nextPos.area()->index(nextPos.worldCoor(), nextPos.index());
		nextPos.setIndex(newIndex);

		if(isBlocked(actor, pos, nextPos)) {
			// Revert back to original area if necessary
			if(nextPos.area() != old) {
				nextPos.setArea(*old, false);
			}
			nextPos.localCoor().x_ = pos.localCoor().x_;
			nextPos.localCoor().z_ = pos.localCoor().z_;
			nextPos.worldCoor().x_ = pos.worldCoor().x_;
			nextPos.worldCoor().z_ = pos.worldCoor().z_;
			nextPos.index_ = pos.index_;
		}

		// Ground height at present position
		coor_t gh = nextPos.area()->groundHeight(nextPos.localCoor(), nextPos.index());
		// If below ground, then make grounded
		nextPos.setGrounded(gh >= nextPos.localCoor().y_);
		// Clamp the character to the ground if grounded
		if(nextPos.isGrounded()) {
			nextPos.localCoor().y_ = gh;
			nextMove.velocity_.y_ = 0;
		}

		nextPos.updateWorldViewPoint();
	}


	bool PhTracker
	::isBlocked(const Actor& actor, const Pos& pos, const Pos& nextPos) const {
		if(!nextPos.hasIndex()) {
			return true;
		}
		if(!nextPos.area()->isLegalCoor(nextPos.worldCoor())) {
			return true;
		}
		short to = nextPos.terrainStyle();
		if(to == NavMeshArea::TS_VOID) {
			return true;
		}

		return false;
	}

}
