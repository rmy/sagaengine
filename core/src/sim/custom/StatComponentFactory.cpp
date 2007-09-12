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


#include "StatComponentFactory.hpp"
#include "StatComponent.hpp"
#include "../sim.hpp"
#include "../SimEngine.hpp"
#include "../config/all.hpp"
#include "../stat/all.hpp"


namespace se_core {

	StatComponentFactory
	::StatComponentFactory()
		: ComponentFactory(sct_STAT), collectibles_(0), shouldSave_(false) {
	}


	StatComponentFactory
	::~StatComponentFactory() {
	}


	void StatComponentFactory
	::setAbilities(short speed, short attack, short defense, short level) {
		short bases[4];
		bases[0] = speed;
		bases[1] = attack;
		bases[2] = defense;
		bases[3] = level;

		abilities_.setBases(bases);
	}


	Component* StatComponentFactory
	::create(Composite* owner) const {
		// Try to get existing component 
		// - allows overrides of default values
		// (Useful when loading saved games).
		StatComponent::Ptr pStats(*owner);
		if(pStats == 0) {
			// Create new component
			pStats = new StatComponent(owner, this);
		}
		pStats->abilities()->setBases(&abilities_);
		pStats->properties_ = &properties_;

		if(shouldSave_) {
			pStats->setShouldSave(true);
		}

		return pStats;
	}

}
