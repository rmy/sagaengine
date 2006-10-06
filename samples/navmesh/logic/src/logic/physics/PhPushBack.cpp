#include "PhPushBack.hpp"
#include "basic/area/NavMeshArea.hpp"

using namespace se_basic;
using namespace se_core;

namespace logic {

	const PhPushBack physicsPushBack;


	void PhPushBack
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
		Vector3 gravity(0, -4 * (COOR_RES / TIMESTEP_INTERVAL), 0);

		// Calculate the change in the coordinate
		// that should affect the mover this tick
		Vector3 v(nextMove.velocity_);

		// Calculate the next position from the present
		// position and its change
		nextPos.localCoor().add(v);

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

		// Calc navigation mesh triangle id
		short newIndex = nextPos.area()->index(nextPos.worldCoor(), nextPos.index());
		nextPos.setIndex(newIndex);


		if(isBlocked(actor, pos, nextPos)) {
			// Revert back to original area if necessary
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


	bool PhPushBack
	::isBlocked(const Actor& actor, const Pos& pos, const Pos& nextPos) const {
		if(!nextPos.hasIndex()) {
			WasHere();
			return true;
		}
		if(!nextPos.area()->isLegalCoor(nextPos.worldCoor())) {
			WasHere();
			return true;
		}
		short to = nextPos.terrainStyle();
		if(to == NavMeshArea::TS_VOID) {
			WasHere();
			return true;
		}

		return false;
	}

	
	void PhPushBack
	::affect(Actor& actor) const {
		if(!actor.didMove()) {
			actor.popPhysics();
		}
		Pos& nextPos = actor.nextPos();
		if(!nextPos.hasArea()) {
			// PushBack has left all areas - which means
			// it should be destroyed
			actor.scheduleForDestruction();
		}
	}


}
