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


#include "ThingCollide.hpp"
#include "../schema/SimSchema.hpp"
#include "../stat/SortedSimObjectList.hpp"
#include "../stat/Pos.hpp"
#include "../thing/Actor.hpp"



namespace se_core {

	ThingCollide
	::ThingCollide(const char* name) : SimObject(got_THING_COLLIDE, name) {
		SimSchema::sortedSimObjectList().add(this);
	}


	bool ThingCollide
	::isGuilty(Actor& pusher, Thing& target) const {
		// Is this thing's movement bringing it closer, or farther away?
		coor_double_t beforeDistance = pusher.pos().worldCoor().xzDistanceSquared(target.pos().worldCoor());
		coor_double_t distanceWithPusherMoving = pusher.nextPos().worldCoor().xzDistanceSquared(target.pos().worldCoor());
		if(distanceWithPusherMoving > beforeDistance) {
			// Not closer, any collision is not caused by this thing
			return false;
		}
		//LogMsg("Guilty " << pusher.name() << " vs " << target.name());
		return true;
	}

}


