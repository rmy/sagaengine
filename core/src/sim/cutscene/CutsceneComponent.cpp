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
#include "comp/list/NodeComponentList.hpp"
#include "util/error/Log.hpp"
#include "../script/ShowingCutscene.hpp"
#include "../script/Cutscene.hpp"



namespace se_core {
	CutsceneComponent
	::CutsceneComponent(Composite* owner, const ComponentFactory* factory)
		: AreaChildComponent(sct_CUTSCENE, owner)
//			, showingCutscene_(0)
	{
	}


	CutsceneComponent
	::~CutsceneComponent() {
		cleanup();
	}


	void CutsceneComponent
	::cleanup() {
	}


	/*
	void Actor
	::stopShowingCutscene() {
		if(showingCutscene_) {
			showingCutscene_->freeMembers();
		}
	}

	void Actor
	::setNoCutsceneShowing() {
		if(showingCutscene_) {
			popScript();
			showingCutscene_ = 0;
		}
	}

	*/

}
