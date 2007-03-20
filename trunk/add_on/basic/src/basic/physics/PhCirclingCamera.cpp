#include "PhCirclingCamera.hpp"
#include "sim/area/Area.hpp"
#include "../area/NavMeshArea.hpp"

using namespace se_basic;
using namespace se_client;
using namespace se_core;

namespace se_basic {

	const PhCirclingCamera physicsCirclingCamera;	


	void PhCirclingCamera
	::calcNext(const PhysicsComponent& physics
			, const Pos& pos
			, Pos& nextPos
			, const Move& move
			, Move& nextMove
			) const {
	
		// If the camera is not inside an area, physics are meaningless
		if(!pos.hasArea()) {
			return;
		}

		if(!nextPos.hasParent()) {
			return;
		}

		// Player has already been updated, because it is set as
		// the parent by the "T" option in the data/logic/start/start.txt
		PosComponent::Ptr target(*nextPos.parent());
		const Pos& targetPos = target->nextPos();

		// As this camera dangles behind the player, the player should
		// be inside an area as well
		if(!targetPos.hasArea()) {
			return;
		}

		// If they are on the same position, move camera
		// to the players spawn coordinate 0
		//SpawnComponent::Ptr targetSpawn(*target);
		//ViewPoint lookAt(*playerSpawn->spawnPoint(4));
		//lookAt.add(playerPos.world_);
		ViewPoint lookAt(targetPos.world_);

		// Keep the camera inside the same area as the player.
		// (This camera cannot collide, so it doesn't matter if the 
		// camera coordinate is outside area bounds.)
		PosComponent* a = target->nextPos().area();
		if(a != 0 && a != nextPos.area()) {
			nextPos.setArea(*a);
		}

		NavMeshArea* area = static_cast<NavMeshArea*>(ClientSchema::player->nextPos().area()->toArea());
		Point3 dest(0, 6 * COOR_RES, 0);
		Vector3 tmp;
		tmp.sub(lookAt.coor_, pos.worldCoor());
		tmp.y_ = 0;
		coor_t dist = tmp.length();
		bray_t yaw = BrayT::invert(pos.worldFace().yaw_);
		yaw = BrayT::invert(tmp.yaw());

		coor_t MAX_TRAIL_DISTANCE = 16 * COOR_RES;
		coor_t MIN_TRAIL_DISTANCE = 1 * COOR_RES;
		coor_t MOVE_SPEED = 0.3f;
		coor_t TRAIL_DISTANCE = dist + MOVE_SPEED;
		if(MOVE_SPEED < 0)
			MOVE_SPEED = 1;

		if(TRAIL_DISTANCE > MAX_TRAIL_DISTANCE) {
			TRAIL_DISTANCE = MAX_TRAIL_DISTANCE;
		}
		coor_t legalDist = area->farthestLineOfSight(targetPos, yaw, TRAIL_DISTANCE, 1 * COOR_RES, dest);
		if(dist > legalDist || dist < MIN_TRAIL_DISTANCE || true) {
			if(TRAIL_DISTANCE > legalDist) {
				TRAIL_DISTANCE -= 2 * MOVE_SPEED;
				if(TRAIL_DISTANCE < legalDist) {
					TRAIL_DISTANCE = legalDist;
				}
			}
			//dest.add(areaPos->nextPos().worldCoor());
			dest.setForward(TRAIL_DISTANCE, yaw);
			dest.add(lookAt.coor_);
			dest.y_ = 6 + 2 * (1 - (TRAIL_DISTANCE / MAX_TRAIL_DISTANCE));

			nextPos.worldCoor().set(dest);
		}
		nextPos.worldCoor().eulerTowards(lookAt.coor_, nextPos.worldFace());
		//nextPos.worldFace().yaw_ = BrayT::towards(pos.worldFace().yaw_, targetPos.worldFace().yaw_, BRAY_RES >> 2);

		// Look at the player
		nextPos.updateLocalViewPoint();
	}


	void PhCirclingCamera
	::affect(PhysicsComponent& physics) const {
		if(!PosComponent::Ptr(physics)->nextPos().hasArea()) {
			// Camera has left all areas - which means
			// it should be destroyed
			//physics.owner()->scheduleForDestruction();
		}
	}

}
