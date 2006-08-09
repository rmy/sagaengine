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


#include "ShowingCutscene.hpp"
#include "../stat/MultiSimObject.hpp"
#include "../stat/SimObjectIterator.hpp"
#include "../thing/Actor.hpp"
#include "../../util/error/Log.hpp"

namespace se_core {


	ShowingCutscene
	::ShowingCutscene() : members_(new MultiSimObject()) {
	}


	ShowingCutscene
	::~ShowingCutscene() {
		delete members_;
	}


	void ShowingCutscene
	::addMember(Actor& a) {
		members_->add(a);
	}


	void ShowingCutscene
	::removeMember(Actor& a) {
		members_->remove(a);
		a.setNoCutsceneShowing();
	}


	void ShowingCutscene
	::freeMembers() {
		SimObjectIterator it(*members_);
		while(it.hasNext()) {
			it.nextActor().setNoCutsceneShowing();
		}
		members_->clear();
	}

}

