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
#include "../pos/PosComponent.hpp"


namespace se_core {

	StatComponent
	::StatComponent(Composite* owner, const ComponentFactory* factory)
		: Component(sct_STAT, owner, factory) {
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


	PosComponent* StatComponent
	::targetPos() {
		Composite* t = target_.object();
		if(!t) return 0;
		return PosComponent::get(*t);
	}


	const PosComponent* StatComponent
	::targetPos() const {
		Composite* t = target_.object();
		if(!t) return 0;
		return PosComponent::get(*t);
	}
}
