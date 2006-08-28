#include "LogicPre.hpp"
#include "Disintegrate.hpp"
#include "ShootAcid.hpp"
#include "../schema/LogicSchema.hpp"
#include "basic/area/NavMeshArea.hpp"

using namespace se_basic;
using namespace se_core;


namespace logic {

	short ShootAcid
	::duration(se_core::Actor& performer, se_core::Parameter& parameter) const {
		static const int duration[] = { 1, 16, 32, 1 };
		return duration[ parameter.actionStage() ];
	}


	bool ShootAcid
	::isContinuing(se_core::Actor &performer, se_core::Parameter& parameter) const {
		return (parameter.actionStage() <= 2);
	}


	void ShootAcid
	::perform(long when, Actor& performer, se_core::Parameter& parameter) const {
		spawn(when, performer);
	}



	void ShootAcid
	::spawn(long when, Actor& performer) const {
		// Spawn the missile at spawn point 0
		Actor* shot = reinterpret_cast<Actor*>(performer.spawn("missile.acid", 0, Pos::TSM_VOID));
		if(!shot)
			return;

		if(performer.isPlayer()) {
			//TODO: play sound effect
		}

		// Set movement speed
		coor_t speed = 4 * shot->walkSpeed();
		shot->nextMove().velocity_.setForward(speed, shot->nextPos().localFace());

		// Set movement mode
		shot->nextPos().anim(0).setMovementMode(when, 0);

		// Floating in air
		shot->nextPos().setGrounded(false);

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
