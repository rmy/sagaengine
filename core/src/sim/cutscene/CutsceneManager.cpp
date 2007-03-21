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
#include "sim/schema/SimSchema.hpp"
#include "comp/list/NodeComponentList.hpp"
#include "util/error/Log.hpp"

using namespace se_core;

namespace se_core {
	CutsceneManager
	::CutsceneManager()
		: RootComponent(sct_CUTSCENE) {
	}

	CutsceneManager
	::~CutsceneManager() {
	}


	CutsceneManager&  CutsceneManager
	::singleton() {
		static CutsceneManager instance;
		return instance;
	}


	void CutsceneManager
	::step(long when) {
		/*
		NodeComponentList::Iterator it(children_);
		while(it.hasNext()) {
			CutsceneAreaComponent& c = static_cast<CutsceneAreaComponent&>(it.next());
			//c.propagate();
		}
		*/
	}


	void CutsceneManager
	::initGame() {
	}


	void CutsceneManager
	::cleanupGame() {
	}

}
