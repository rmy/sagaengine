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


#include "NodeComponent.hpp"
#include "comp/Composite.hpp"
#include "util/error/Log.hpp"


namespace se_core {

	NodeComponent
	::NodeComponent(int type, Composite* owner, const ComponentFactory* factory)
		: Component(type, owner, factory), parent_(0) {
	}


	NodeComponent
	::~NodeComponent() {
	}


	void NodeComponent
	::resetParent() {
		if(parent_)
			parent_->removeChild(*this);

		parent_ = 0;
	}


	void NodeComponent
	::setParent(NodeComponent& p) {
		if(parent_)
			parent_->removeChild(*this);

		parent_ = &p;
		parent_->addChild(*this);
	}


	void NodeComponent
	::addChild(NodeComponent& node) {
		// Static pos nodes should have the area as
		// parent.
		children_.add(node);
	}


	void NodeComponent
	::removeChild(NodeComponent& node) {
		children_.remove(node);
	}


	void NodeComponent
	::parentChanged(Composite* newParent, Composite* oldParent) {
		if(newParent) {
			NodeComponent* n = static_cast<NodeComponent*>(newParent->component(type()));
			if(n) {
				setParent(*n);
			}
		}
		else {
			resetParent();
		}	
	}

}
