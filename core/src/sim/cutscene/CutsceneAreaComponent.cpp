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


#include "CutsceneManager.hpp"
#include "CutsceneAreaComponent.hpp"
#include "CutsceneComponent.hpp"
#include "sim/sim.hpp"
#include "sim/schema/SimSchema.hpp"
#include "sim/stat/MultiSimNodeComponent.hpp"
#include "util/error/Log.hpp"



namespace se_core {

	CutsceneAreaComponent
	::CutsceneAreaComponent(Composite* owner, const SimComponentFactory* factory) 
			: AreaComponent(sct_CUTSCENE, owner, factory) {
	}


	CutsceneAreaComponent
	::~CutsceneAreaComponent() {
	}


	void CutsceneAreaComponent
	::setActive(bool state) {
		if(state) {
			SimNodeComponent* c = static_cast<SimNodeComponent*>(CompSchema::activeRoot().component(type_));
			if(c) {
				setParent(*c);
			}
		}
		else {
			resetParent();
		}
	}


	/*
	void Area
	::enter(Actor& performer) {
		Cutscene* c = 0;

		SimObjectList::iterator_type it = multiSimObject(MGOA_CUTSCENES).iterator();
		while(it != SimObjectList::end()) {
			c = SimSchema::simObjectList.nextCutscene(it);
			Assert(c != 0);
			if(!performer.questGoals().contains(c->afterTheseGoals())) {
				c = 0;
				continue;
			}
			if(performer.questGoals().sharesAny(c->beforeTheseGoals())) {
				c = 0;
				continue;
			}
			break;
		}
		if(!c) return;
		if(c->questGoal()) {
			performer.questGoals().add(*(c->questGoal()));
		}
		// TODO:
		//c->setScripts(*this, performer);
	}
	*/


}

