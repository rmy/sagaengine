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


#include "CustomScript.hpp"
#include "../action/Action.hpp"
#include "../thing/Actor.hpp"
#include "../../util/type/String.hpp"
#include "../../util/error/Log.hpp"


namespace se_core {


	CustomScript
	::CustomScript(String* name) : Script(name) {
	}


	const Action* CustomScript
	::nextAction(const Actor& performer, int channel, ScriptData* sd) const {
		switch(channel) {
		case CHANNEL_DIRECTION:
			return nextDirection(performer, sd);
		case CHANNEL_MOVEMENT:
			return nextMovement(performer, sd);
		case CHANNEL_EXTRA:
			return nextExtra(performer, sd);
		}
		return 0;
	}


	ScriptData* CustomScript
	::init(const Actor& performer) const {
		return 0;
	}


	const Action* CustomScript
	::nextMovement(const Actor& performer, ScriptData* sd) const {
		return 0;
	}


	const Action* CustomScript
	::nextDirection(const Actor& performer, ScriptData* sd) const {
		return 0;
	}


	const Action* CustomScript
	::nextExtra(const Actor& performer, ScriptData* sd) const {
		return 0;
	}

}
