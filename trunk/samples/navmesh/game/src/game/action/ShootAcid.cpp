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
#include "Disintegrate.hpp"
#include "ShootAcid.hpp"

using namespace se_core;


namespace game {

	short ShootAcid
	::duration(se_core::Actor& performer, se_core::Parameter& parameter) const {
		return 8;
	}


	bool ShootAcid
	::isContinuing(se_core::Actor &performer, se_core::Parameter& parameter) const {
		return false;
	}


	void ShootAcid
	::perform(long when, Actor& performer, se_core::Parameter& parameter) const {
		spawn(when, performer);
	}


	void ShootAcid
	::spawn(long when, Actor& performer) const {
		// Spawn the missile
		Actor* shot = reinterpret_cast<Actor*>(performer.spawn("missile.acid"));
		if(performer.isPlayer()) {
			//TODO: play sound effect
		}

		// Set movement speed
		coor_t speed = shot->walkSpeed();
		//speed = COOR_RES;
		//bray_t yaw = performer.pos().face();
		Quat4 f(performer.pos().face());
		//shot->nextMove().changeMovement(f, speed);
		shot->nextMove().changeMovement(0, speed);
		//shot->nextPos().setFace(f);

		// Set movement mode
		shot->nextAnim().setMovementMode(when, 0);

		// Floating in air
		shot->nextPos().setNoLayer();

		// Copy abilities of owner
		shot->abilities()->setBases(performer.abilities());

		// Missile should should disappear when past its
		// maxiumum range
		Parameter tmp;
		actionDisintegrate.param(2048, tmp);
		shot->planAction(CHANNEL_EXTRA, actionDisintegrate, &tmp);
	}

	const ShootAcid actionShootAcid;
}
