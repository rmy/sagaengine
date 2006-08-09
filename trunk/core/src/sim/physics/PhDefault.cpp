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
#include "PhysicsUtil.hpp"
#include "../area/Area.hpp"
#include "../stat/all.hpp"
#include "../thing/Actor.hpp"
#include "util/math/all.hpp"


namespace se_core {
	void PhDefault
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

		// Default never moves.
	}


	bool PhDefault
	::isBlocked(const Actor& actor, const Pos& pos, const Pos& nextPos) const {
		short to = nextPos.terrainStyle();
		// 0 = TS_VOID
		if(to <= Pos::TS_VOID) {
			return true;
		}

		return false;
	}

}
