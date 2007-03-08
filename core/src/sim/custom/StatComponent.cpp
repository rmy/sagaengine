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


#include "StatComponent.hpp"
#include "ThingData.hpp"
#include "../sim.hpp"
#include "../SimEngine.hpp"
#include "../config/all.hpp"
#include "../stat/all.hpp"


namespace se_core {

	StatComponent
	::StatComponent(SimComposite* owner, const SimComponentFactory* factory)
		: SimComponent(sct_STAT, owner, factory) {
	}


	coor_t StatComponent
	::walkSpeed() const {
		return (abilities()->modified(Abilities::SPEED) * CoorT::half(COOR_STEP));
	}


	void StatComponent
	::setAbilities(short* abilities) {
	}


	void StatComponent
	::setQuickMenuAction(const Action* a) {
		quickMenuAction_ = a;
	}


	void StatComponent
	::setUseAction(const Action* a) {
		useAction_ = a;
	}

	/*
	void StatComponent
	::setSpecial(enum Special special, long millis) {
		special_ = special;
		specialWhen_ = SimSchema::simEngine.when() + millis;
	}


	bool StatComponent
	::isSpecialEndingSoon() const {
		if(specialWhen_ < SimSchema::simEngine.when()) {
			return false;
		}
		if(specialWhen_ < SimSchema::simEngine.when() + 4 * 1024) {
			return true;
		}
		return false;
	}


	enum StatComponent::Special StatComponent
	::special() const {
		if(specialWhen_ < SimSchema::simEngine.when()) {
			return NONE;
		}
		return special_;
	}
	*/
}
