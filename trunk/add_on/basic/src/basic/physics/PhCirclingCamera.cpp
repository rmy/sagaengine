#include "PhCirclingCamera.hpp"
#include "sim/area/Area.hpp"

using namespace se_basic;
using namespace se_client;
using namespace se_core;

namespace se_basic {

	const PhCirclingCamera physicsCirclingCamera;	


	void PhCirclingCamera
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
		PosComponent* a = ClientSchema::player->nextPos().area();
		if(a != 0 && a != nextPos.area()) {
			nextPos.setArea(*a);
		}

		// Stay glued to the players spawn point 0
		if(nextPos.local_.coor().isZero()) {
			
			const ViewPoint* sp = SpawnComponent::get(*ClientSchema::player)->spawnPoint(0);
			nextPos.local_.setViewPoint(*sp);
		}

		bray_t yaw = nextPos.local_.face().yaw_;

		nextPos.updateWorldViewPoint();


		ViewPoint& parentNextPos = nextPos.parent()->nextPos().world_;

		nextMove.angularVelocity_.scale(0.9f);
		nextMove.angularVelocity_.rotate(move.torque_);

		nextPos.world_.face().pitch_ = 0 * BRAY_RES;
		nextPos.world_.face().roll_ = 0 * BRAY_RES;
		nextPos.world_.face().yaw_ = parentNextPos.face_.yaw_ + yaw + move.angularVelocity_.yaw_;
		nextPos.world_.coor().y_ = parentNextPos.coor_.y_;
		Vector3 tmp;
		tmp.setForward(-COOR_RES, nextPos.world_.face().yaw_);
		nextPos.world_.coor().set( parentNextPos.coor_ );
		nextPos.world_.coor().add( tmp );

		nextPos.updateLocalViewPoint();

		/* nextPos.local_.face().yaw_ = yaw + move.angularVelocity_.yaw_; nextPos.local_.coor().reset(); nextPos.updateWorldViewPoint(); */

		//LogMsg("Y: " << nextPos.world_.coor_.y_ << " - " << nextPos.local_.coor_.y_ << ": " << nextPos.parent()->nextPos().world_.coor_.y_);
		//LogMsg("Y: " << pos.world_.coor_.y_ << " - " << pos.local_.coor_.y_ << ": " << pos.parent()->pos().world_.coor_.y_);
		//LogMsg("Y: " << nextPos.world_.coor_.y_ << " - " << nextPos.local_.coor_.y_);
	}


	void PhCirclingCamera
	::affect(Actor& actor) const {
		Pos& nextPos = actor.nextPos();
		if(!nextPos.hasArea()) {
			// Camera has left all areas - which means
			// it should be destroyed
			actor.scheduleForDestruction();
		}
	}

}
