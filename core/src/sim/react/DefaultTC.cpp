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


#include "DefaultTC.hpp"
#include "../thing/Actor.hpp"
#include "../action/all.hpp"



namespace se_core {

	DefaultTC
	::DefaultTC() : ThingCollide("default") {
	}

	bool DefaultTC
	::collide(CollisionComponent& pusher, const CollisionComponent& target) const {
		if(pusher.shouldIgnore(target))
			return false;
		if(isGuilty(pusher, target)) {
			return true;
		}
		return false;
	}

	const DefaultTC tcDefault;
}
