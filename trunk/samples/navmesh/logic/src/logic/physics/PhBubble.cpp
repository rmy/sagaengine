#include "PhBubble.hpp"
#include "basic/area/NavMeshArea.hpp"

using namespace se_basic;
using namespace se_core;

namespace logic {

	const PhBubble physicsBubble;	


	void PhBubble
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

		Vector3 gravity(0, -0.5 * (COOR_RES / TIMESTEP_INTERVAL), 0);
		nextMove.velocity_.add( gravity );
		nextMove.velocity_.add( move.force_ );
		nextMove.angularVelocity_.add( move.torque_ );

		// Calculate the change in the coordinate
		nextPos.localCoor().add(nextMove.velocity_);

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

		// If below ground, then destroy missile
		coor_t gh = nextPos.area()->groundHeight(nextPos.localCoor(), nextPos.index());
		nextPos.setGrounded(gh >= nextPos.localCoor().y_);

		// Clamp the character to the ground if grounded
		if(nextPos.isGrounded()) {
			nextPos.localCoor().y_ = gh;
			nextMove.velocity_.y_ = 0;
		}

		if(isBlocked(actor, pos, nextPos)) {
			// Cant destroy it before affect, so just leave area here
			nextPos.resetArea();
		}

		nextPos.updateWorldViewPoint();
	}


	bool PhBubble
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

	
	void PhBubble
	::affect(Actor& actor) const {
		Pos& nextPos = actor.nextPos();
		if(!nextPos.hasArea()) {
			// Bubble has left all areas - which means
			// it should be destroyed
			actor.scheduleForDestruction();
		}
	}
	
}
