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


#include "Script.hpp"
#include "ScriptComponent.hpp"
#include "../sim.hpp"
#include "../action/Action.hpp"
#include "../schema/SimSchema.hpp"
#include "../../util/type/String.hpp"
#include "../../util/error/Log.hpp"


namespace se_core {

	DebugExec(int ScriptData::refCount = 0);

	ScriptData
	::ScriptData() {
		DebugExec(++refCount);
	}

	ScriptData
	::~ScriptData() {
		DebugExec(--refCount);
	}


	Script
	::Script(const char* name)
			: Object(got_SCRIPT, name)
			, nameString_(name) {
	}


	Script
	::~Script() {
		//SimSchema::sortedSimObjectList().remove(this);
		//delete nameString_;
	}


	void Script
	::nextAction(const ScriptComponent& performer, int channel, ScriptData* sd, ActionAndParameter& out) const {
		Parameter& p = out.parameter();
		const Action* a = nextAction(performer, channel, sd, p);
		if(a) {
			out.setAction(*a);
		}
	}


	void Script
	::trackUserFeedback() const {
		WasHere();
		SimSchema::didTrack = true;
	}

}
