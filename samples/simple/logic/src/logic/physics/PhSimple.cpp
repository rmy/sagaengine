#include "PhSimple.hpp"
#include "basic/area/NavMeshArea.hpp"


using namespace se_basic;
using namespace se_core;

namespace logic {
	const PhSimple physicsSimple("Simple");


	void PhSimple
	::calcNext(const PhysicsComponent& physics
			, const Pos& pos
			, Pos& nextPos
			, const Move& move
			, Move& nextMove
			) const {

		nextMove.addForce( GRAVITY );
		PhUtil::calcNext(physics, pos, nextPos, move, nextMove);
	}


	void PhSimple
	::affect(PhysicsComponent& physics) const {
	}


}
