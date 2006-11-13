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


#include "SimComponent.hpp"
#include "SimNodeComponent.hpp"
#include "SimObject.hpp"
#include "SimComposite.hpp"
#include "schema/SimSchema.hpp"
#include "util/error/Log.hpp"


namespace se_core {

	SimNodeComponent
	::SimNodeComponent(SimComposite* owner)
		: SimComponent(sct_NODE, owner), parent_(0) {
	}


	SimNodeComponent
	::SimNodeComponent(enum SimComponentType type, SimComposite* owner)
		: SimComponent(type, owner), parent_(0) {
	}


	SimNodeComponent
	::~SimNodeComponent() {
	}


	void SimNodeComponent
	::resetParent() {
		if(parent_)
			parent_->removeChild(*this);

		parent_ = 0;
	}


	void SimNodeComponent
	::setParent(SimNodeComponent& p) {
		if(parent_)
			parent_->removeChild(*this);

		parent_ = &p;
		parent_->addChild(*this);
	}


	void SimNodeComponent
	::addChild(SimNodeComponent& node) {
		// Static pos nodes should have the area as
		// parent.
		children_.add(node);
	}


	void SimNodeComponent
	::removeChild(SimNodeComponent& node) {
		children_.remove(node);
	}


	void SimNodeComponent
	::parentChanged(SimComposite* newParent, SimComposite* oldParent) {
		if(newParent) {
			SimNodeComponent* n = static_cast<SimNodeComponent*>(newParent->component(type()));
			if(n) {
				setParent(*n);
			}
		}
		else {
			resetParent();
		}
	}

}
