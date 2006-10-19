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


#include "SimComposite.hpp"
#include "SimNodeComposite.hpp"
#include "SimObject.hpp"
#include "thing/Actor.hpp"
#include "schema/SimSchema.hpp"
#include "stat/SimCompositeList.hpp"
#include "util/error/Log.hpp"


namespace se_core {

	SimNodeComposite
	::SimNodeComposite(Actor* owner)
		: SimComposite(sct_NODE, owner), firstChild_(-1) {
	}


	SimNodeComposite
	::~SimNodeComposite() {
	}


	bool SimNodeComposite
	::changeParent() {
		leaveCurrentParent();

		if(hasParent()) {
			parent()->addChild(*this);
		}
		else {
			return false;
		}
		return true;
	}


	void SimNodeComposite
	::leaveCurrentParent() {
		if(hasParent()) {
			parent()->removeChild(*this);
		}
	}


	void SimNodeComposite
	::addChild(SimNodeComposite& node) {
		// Static pos nodes should have the area as
		// parent.
		SimSchema::simCompositeList.add(node, firstChild_);
	}


	void SimNodeComposite
	::removeChild(SimNodeComposite& node) {
		SimSchema::simCompositeList.remove(node, firstChild_);
	}

}
