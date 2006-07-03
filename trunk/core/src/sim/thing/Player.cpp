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
		  , defaultAction_(0), defaultMovementAction_(&actionIdle)
		  , defaultTurnAction_(0)
		  , pickTarget_(0), defaultActionTarget_(0) {
		reportingMultiSimObjects_ = new ReportingMultiSimObject*[ MGO_COUNT ];
		for(int i = 0; i < MGO_COUNT; ++i)
			reportingMultiSimObjects_[i] = new ReportingMultiSimObject(*this, i);
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
		clearScripts();
		for(int i = 0; i < CHANNEL_COUNT; ++i) {
			clearPlannedAction(i);
		}
		disrupt();
		/*
		SimObjectList::iterator_type it = carriedThings().iterator();
		while(it != SimObjectList::NULL_NODE) {
			Thing* t = SimSchema::simObjectList.nextThing(it);
			t->scheduleForDestruction();
			}
		*/
		isDead_ = true;
	}


	void Player
	::reallyScheduleForDestruction() {
		scheduleForDestruction();
		SimSchema::thingManager().scheduleForDestruction(*this);
	}


	bool Player
	::changeArea() {
		lastEntrance_.setCoor(nextPos());
		return Camera::changeArea();
	}



	void Player
	::planDefaultAction() const {
		if(!defaultAction_) return;
		planAction(CHANNEL_EXTRA, *defaultAction_);
	}


	void Player
	::planDefaultMovementAction() const {
		if(defaultMovementAction_) {
			if(!plannedAction_[ CHANNEL_MOVEMENT ].hasAction()) {
				planAction(CHANNEL_MOVEMENT, *defaultMovementAction_);
			}
		}

		if(defaultTurnAction_) {
			if(!plannedAction_[ CHANNEL_DIRECTION ].hasAction()) {
				planAction(CHANNEL_DIRECTION, *defaultTurnAction_);
			}
		}
	}


	bool Player
	::findDefaultActionTarget() {
		// Asser(!isDead_);
		if(area()) {
			return (area()->findDefaultActionTarget(*this) != 0);
		}
		return false;
	}


	void Player
	::findPickTarget() {
		// Asser(!isDead_);
		if(area()) area()->findPickTarget(*this);
	}


	void Player
	::resetIfTarget(Thing* target) {
		if(defaultActionTarget_ == target) defaultActionTarget_ = 0;
		if(pickTarget_ == target) pickTarget_ = 0;
	}

}
