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


#include "ReentrantScript.hpp"
#include "../action/Action.hpp"
#include "../action/Idle.hpp"
#include "ScriptComponent.hpp"
#include "../../util/type/String.hpp"
#include "../../util/error/Log.hpp"



namespace se_core {

	ReentrantScript
	::ReentrantScript(const char* name, int channel) : Script(name), channel_(channel) {
	}


	const Action* ReentrantScript
	::nextAction(const ScriptComponent& performer, int channel, ScriptData* sd, Parameter& out) const {
		// No simultaneous actions for reentrant scripts. Always
		// put actions in action queue channel 0.
		if(channel != channel_)
			return 0;

		Assert(sd);
		ReentrantData& rd = static_cast<ReentrantData&>(*sd);
		const Action* a = transition(performer, rd, out);
		if(!a) {
			a = sequence(performer, rd, out);
		}
		return a;
	}

}
