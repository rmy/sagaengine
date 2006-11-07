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


#include "Player.hpp"
#include "ThingManager.hpp"
#include "../SimEngine.hpp"
#include "../action/ActionQueue.hpp"
#include "../action/ActionAndParameter.hpp"
#include "../action/Idle.hpp"
#include "../area/Area.hpp"
#include "../config/all.hpp"
#include "../message/MessageCentral.hpp"
#include "../schema/SimSchema.hpp"
#include "../stat/SimObjectList.hpp"
#include "../stat/MultiSimObject.hpp"
#include "../stat/ReportingMultiSimObject.hpp"
#include "util/error/Log.hpp"



namespace se_core {

	Player
	::Player(const char* name)
		: Camera(name)
		  , pickTarget_(0), defaultActionTarget_(0) {
		reportingMultiSimObjects_ = new ReportingMultiSimObject*[ MGO_COUNT ];
		for(int i = 0; i < MGO_COUNT; ++i)
			reportingMultiSimObjects_[i] = new ReportingMultiSimObject(*this, i);
		defaultMovementAction_.setAction(actionIdle);
	}


	Player
	::~Player() {
		for(int i = 0; i < MGO_COUNT; ++i)
			delete reportingMultiSimObjects_[i];
		delete[] reportingMultiSimObjects_;
	}


	/**
	 * @param type Thing::MultiSimObjectType
	 */
	MultiSimObject& Player
	::multiSimObject(short type) {
		return *reportingMultiSimObjects_[type];
	}


	const MultiSimObject& Player
	::multiSimObject(short type) const {
		return *reportingMultiSimObjects_[type];
	}


	ReportingMultiSimObject& Player
	::reportingMultiSimObject(short type) {
		//Assert(&dynamic_cast<ReportingMultiSimObject&>(multiSimObject(type)) != 0);
		return *reportingMultiSimObjects_[type];
	}


	void Player
	::scheduleForDestruction() {
		if(isDead_) return;

		SimSchema::simEngine.setGameOver(true);
		scriptComponent_->clearScripts();
		actionComponent_->cleanup();
		isDead_ = true;
	}


	void Player
	::reallyScheduleForDestruction() {
		scheduleForDestruction();
		SimSchema::thingManager().scheduleForDestruction(*this);
		isDead_ = true;
	}


	bool Player
	::changeArea() {
		if(isDead_) return false;
		lastEntrance_.setViewPoint(nextPos().local_);
		return Camera::changeArea();
	}


	void Player
	::planAction(short channel, const Action& action, const Parameter* parameter) const {
		actionComponent_->planAction(channel, action, parameter);
	}

	void Player
	::planDefaultMovementAction() const {
		if(defaultMovementAction_.hasAction()) {
			actionComponent_->planActionIfNone(CHANNEL_MOVEMENT, defaultMovementAction_);
		}

		if(defaultTurnAction_.hasAction()) {
			actionComponent_->planActionIfNone(CHANNEL_DIRECTION, defaultTurnAction_);
		}
	}


	void Player
	::performDefaultMovementAction() const {
		long when = SimSchema::simEngine.when();
		WasHere();
		if(defaultMovementAction_.hasAction()) {
			const Action* a = defaultMovementAction_.action();
			Parameter& p = defaultMovementAction_.parameter();
			LogMsg(a->name());
			a->perform(when, *actionComponent_, p);
		}
		if(defaultTurnAction_.hasAction()) {
			const Action* a = defaultTurnAction_.action();
			Parameter& p = defaultTurnAction_.parameter();
			a->perform(when, *actionComponent_, p);
		}
	}


	bool Player
	::findDefaultActionTarget() {
		if(pos().hasArea()) {
			return (pos().area()->findDefaultActionTarget(*this) != 0);
		}
		return false;
	}


	void Player
	::findPickTarget() {
		if(pos().hasArea()) pos().area()->findPickTarget(*this);
	}


	void Player
	::resetIfTarget(Thing* target) {
		if(defaultActionTarget_ == target) defaultActionTarget_ = 0;
		if(pickTarget_ == target) pickTarget_ = 0;
	}

}
