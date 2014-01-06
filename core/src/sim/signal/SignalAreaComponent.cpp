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


#include "../sim.hpp"
#include "SignalAreaComponent.hpp"
#include "SignalManager.hpp"
#include "SignalComponent.hpp"
#include "../schema/SimSchema.hpp"
#include "comp/list/NodeComponentList.hpp"
#include "../react/CollisionAreaComponent.hpp"
#include "../react/CollisionComponent.hpp"
#include "util/error/Log.hpp"
#include "util/bounds/BoundingBox.hpp"
#include "../thing/Actor.hpp"



namespace se_core {

	SignalAreaComponent
	::SignalAreaComponent(Composite* owner) 
			: RootChildComponent(sct_SIGNAL, owner), changed_(0) {
		for(int i = 0; i < MAX_SIGNALS; ++i) {
			inactiveSignals_[i] = 0;
		}
	}


	SignalAreaComponent
	::~SignalAreaComponent() {
	}


	void SignalAreaComponent
	::init() {
	}

	void SignalAreaComponent
	::cleanup() {
		changed_ = 0;
		for(int i = 0; i < MAX_SIGNALS; ++i) {
			inactiveSignals_[i] = 0;
		}
	}

	void SignalAreaComponent
	::setActive(bool state) {
		if(state) {
			NodeComponent* c = static_cast<NodeComponent*>(CompSchema::activeRoot().component(type_));
			if(c) {
				setParent(*c);
			}
		}
		else {
			resetParent();
		}
	}


	void SignalAreaComponent
	::initSignalActive(int id, bool state) {
		Assert(id >= 0 && id < MAX_SIGNALS);
		if(!state) {
			if(inactiveSignals_[id] == 0) {
				changed_ = 1L << id;
			}
			++inactiveSignals_[id];
		}
		Assert(inactiveSignals_[id] >= 0);
	}


	void SignalAreaComponent
	::setSignalActive(int id, bool state) {
		Assert(id >= 0 && id < MAX_SIGNALS);
		if(state) {
			--inactiveSignals_[id];
			if(inactiveSignals_[id] == 0) {
				changed_ = 1 << id;
			}
		}
		else {
			if(inactiveSignals_[id] == 0) {
				changed_ = 1 << id;
			}
			++inactiveSignals_[id];
		}
		Assert(inactiveSignals_[id] >= 0);
		LogWarning("Set signal state: " << id << " = " << state << ": " << inactiveSignals_[id] << " - " << owner()->name());
	}


	void SignalAreaComponent
	::propagate() {
		for(int i = 0; i < MAX_SIGNALS; ++i) {
			if(changed_ & (1 << i)) {
				propagate(i, inactiveSignals_[i] == 0);
			}
		}
		changed_ = 0;
	}


	void SignalAreaComponent
	::propagate(int id, bool state) {
		NodeComponentList::Iterator it(children_);
		while(it.hasNext()) {
			SignalComponent& c = static_cast<SignalComponent&>(it.next());
			c.recieve(id, state);
		}
	}
}
