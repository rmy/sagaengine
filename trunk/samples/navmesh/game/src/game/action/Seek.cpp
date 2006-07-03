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


#include "GamePre.H"
#include "Seek.hpp"
#include "../area/NavMeshArea.hpp"

using namespace se_core;


namespace game {

	void Seek
	::perform(long when, Actor& performer, Parameter& parameter) const {
		if(!performer.hasTarget() || !performer.pos().hasArea()) {
			return;
		}
		const NavMeshArea* area = static_cast<const NavMeshArea*>(performer.pos().area());
		Actor* target = performer.target();

		if(target->pos().area() != area) {
			return;
		}

		//LogMsg("Target: " << target->pos().x_ << ", " << target->pos().z_ << ": " << target->pos().index());
		//LogMsg("Performer: " << performer.pos().x_ << ", " << performer.pos().z_ << ": " << performer.pos().index());

		Point3 tmp;
		area->path(performer.pos(), target->pos(), tmp);

		//LogMsg(tmp.x_ << ", " << tmp.z_);

		Vector3 dir(tmp);
		dir.sub(performer.pos());

		coor_t s1 = performer.walkSpeed() * 0.5;
		coor_t s2 = dir.length();
		coor_t s = (s1 < s2) ? s1 : s2;

		bray_t d = performer.pos().xzAngleTowards(tmp);
		performer.nextMove().changeMovement(BrayT::mask(d), s);
	}

	const Seek actionSeek;
}
