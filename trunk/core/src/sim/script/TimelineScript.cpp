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


#include "TimelineScript.hpp"
#include "ScriptComponent.hpp"
#include "../sim.hpp"
#include "../SimEngine.hpp"
#include "../action/Action.hpp"
#include "../action/Idle.hpp"
#include "../schema/SimSchema.hpp"
#include "../stat/SortedSimObjectList.hpp"
#include "../../util/type/String.hpp"
#include "../../util/error/Log.hpp"


namespace se_core {


	TimelineScript
	::TimelineScript(const char* name, int trackerPosCount) : Script(name), trackerPosCount_(trackerPosCount), isInZeroMode_(trackerPosCount == 1) {
		actions_ = new const Action*[ trackerPosCount_ ];
		scriptSingleValueParameters_ = new int[ trackerPosCount_ ];
		scriptAttributeParameters_ = new String*[ trackerPosCount_ ];
		for(int j = 0; j < trackerPosCount_; ++j) {
			actions_[j] = 0;
			scriptSingleValueParameters_[j] = 0;
			scriptAttributeParameters_[j] = 0;
		}
	}


	TimelineScript
	::~TimelineScript() {
		delete actions_;
	}


    void TimelineScript
	::addAction(int trackerPos, const Action* action) {
		Assert(trackerPos < trackerPosCount_);
		actions_[ trackerPosCount_ + trackerPos ] = action;
	}


	void TimelineScript
	::addAction(int trackerPos, const char* name) {
		Assert(trackerPos < trackerPosCount_);
		addAction(trackerPos, SimSchema::sortedSimObjectList().action(name));
	}


	const Action* TimelineScript
	::nextAction(const ScriptComponent& performer, int channel, ScriptData* sd, Parameter& out) const {
		if(channel != 0)
			return 0;
		if(isInZeroMode_) {
			const Action* a = actions_[ trackerPosCount_ + 0];
			if(!a) { return &actionIdle; }
			//performer.param(channel).setNumber(scriptSingleValueParameters_[0]);
			if(scriptAttributeParameters_[0]) {
				//performer.param(channel).setString(scriptAttributeParameters_[0]->get());
			}
			return a;
		}

		//LogDetail((sprintf(log_msg(), "Tracker: %d - %d - %d - %s", SimSchema::scriptTracker, SimSchema::simEngine.when(), channel, performer.name()), log_msg()));
		Assert(SimSchema::scriptTracker < trackerPosCount_);
		const Action* a = actions_[ trackerPosCount_ + SimSchema::scriptTracker];
		if(!a) { return &actionIdle; }
		SimSchema::didTrack = true;
		//performer.param(channel).setNumber(scriptSingleValueParameters_[SimSchema::scriptTracker]);
		if(scriptAttributeParameters_[SimSchema::scriptTracker]) {
			//performer.param(channel).setString(scriptAttributeParameters_[SimSchema::scriptTracker]->get());
			//LogDetail(performer.param(channel).string());
		}
		return a;
	}

}
