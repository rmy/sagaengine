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


#include "Action.hpp"
#include "../schema/SimSchema.hpp"
#include "../thing/Actor.hpp"
#include <cstdio>


namespace se_core {

	Action
	::Action(const char* name) : Object(got_ACTION, name) {
		//registerInSortedList();
	}


	void Action
	::sound(ActionComponent &performer) const {
		char s[256];
		sprintf(s, "%s.%s", performer.name(), name());
		Actor::Ptr(performer)->sound(s);
	}

}
