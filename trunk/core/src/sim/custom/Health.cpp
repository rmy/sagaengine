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


#include "Health.hpp"
#ifndef SE_OWN_HEALTH
#include "../stat/HealthListeners.hpp"
#include "../schema/SimSchema.hpp"
#include "../thing/Actor.hpp"


namespace se_core {

	Health
	::Health() : when_(0), maxHitpoints_(15), currentHitpoints_(15), hitpointRecoveryRate_(13) {
	}


	short Health
	::maxHitpoints() const {
		return maxHitpoints_;
	}


	short Health
	::currentHitpoints(long when) const {
		if(when_ == 0)
			return currentHitpoints_;
		int ch = currentHitpoints_ + ((when - when_) >> hitpointRecoveryRate_);
		if(ch > maxHitpoints_)
			ch = maxHitpoints_;
		return ch;
	}


	void Health
	::adjustHitpoints(Actor& actor, long when, int amount) {
		currentHitpoints_ = currentHitpoints(when);
		currentHitpoints_ += amount;
		when_ = when;
		SimSchema::healthListeners().castHealthChangedEvent(actor, when, amount);
	}
}

#endif // SE_OWN_HEALTH
