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


#include "LogicPre.hpp"
#include "CoWitchPortal.hpp"
#include "../action/Sprint.hpp"
#include "sim/action/ActionComponent.hpp"
#include "sim/pos/PosComponent.hpp"
#include "sim/physics/PhysicsComponent.hpp"
#include "sim/react/CollisionComponent.hpp"
#include "sim/action/ActionComponent.hpp"

using namespace se_core;

namespace logic {

	CoWitchPortal
	::CoWitchPortal() : ThingCollide("WitchPortal") {
	}


	bool CoWitchPortal
	::collide(se_core::CollisionComponent& pusher
			  , const se_core::CollisionComponent& target) const {
		ActionComponent::Ptr pAction(pusher);
		pAction->planAction(CHANNEL_EXTRA, actionSprint);
		return false;
	}


	const CoWitchPortal coWitchPortal;
}
