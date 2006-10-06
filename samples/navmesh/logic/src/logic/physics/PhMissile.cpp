#include "PhMissile.hpp"
#include "basic/area/NavMeshArea.hpp"

using namespace se_basic;
using namespace se_core;

namespace logic {

	void PhMissile
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

		// Calculate the change in the coordinate
		// that should affect the mover this tick
		nextPos.localCoor().add(nextMove.velocity_);

		// Entered new area?
		Area* old = nextPos.area();
		if(!nextPos.area()->isLegalCoor(nextPos.worldCoor())) {
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

		// If blocked, then destroy missile
		if(isBlocked(actor, pos, nextPos)) {
			// Cant destroy it before affect, so just leave area here
			nextPos.resetArea();
		}

		nextPos.updateWorldViewPoint();
		//LogMsg(actor.name() << ": " <<  actor.pos().worldCoor().y_ << ", " << nextPos.worldCoor().y_);
	}


	bool PhMissile
	::isBlocked(const Actor& actor, const Pos& pos, const Pos& nextPos) const {
		if(!nextPos.hasIndex()) {
			return true;
		}
		if(!nextPos.area()->isLegalCoor(nextPos.worldCoor())) {
			return true;
		}
		coor_t gh = nextPos.area()->groundHeight(nextPos.localCoor(), nextPos.index());
		if(gh > nextPos.localCoor().y_) {
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
