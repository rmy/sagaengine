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
#include "TargetPlayer.hpp"


using namespace se_core;

namespace game {


	void TargetPlayer
	::perform(long when, Actor& performer, Parameter& parameter) const {
		MultiSimObjectIterator it(performer.area()->allThings());
		while(it.hasNext()) {
			Thing* t = &it.nextThing();
			if(t->isType(got_PLAYER)) {
				performer.setTarget(static_cast<Actor*>(t));
				break;
			}
		}
	}


	bool TargetPlayer
	::isContinuing(Actor& performer, Parameter& parameter) const {
		return (!performer.hasTarget());
	}


	const TargetPlayer actionTargetPlayer;
}
