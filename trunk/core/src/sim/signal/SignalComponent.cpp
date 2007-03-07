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


#include "SignalComponent.hpp"
#include "SignalAreaComponent.hpp"
#include "../SimEngine.hpp"
#include "../action/all.hpp"
#include "../thing/Actor.hpp"
#include "../schema/SimSchema.hpp"
#include "../stat/SortedSimObjectList.hpp"
#include "util/type/String.hpp"
#include "util/error/Log.hpp"


namespace se_core {
	SignalComponent
	::SignalComponent(SimComposite* owner, const SimComponentFactory* factory)
		: AreaChildComponent(sct_SIGNAL, owner), sendState_(false), sendId_(1), sentWhen_(0), recieveMask_(0)
		, signal_(0) {
	}


	SignalComponent
	::~SignalComponent() {
		cleanup();
	}


	void SignalComponent
	::cleanup() {
	}


	bool SignalComponent
	::send(bool state) {
		sentWhen_ = SimSchema::simEngine.when();
		if(sendState_ == state)
			return false;
		sendState_ = state;

		SignalAreaComponent* areaSignal = SignalAreaComponent::get(*parent());
		areaSignal->setSignalActive(sendId_, sendState_);
		return true;
	}

	long SignalComponent
	::sentAge() const {
		return (SimSchema::simEngine.when() - sentWhen_);
	}

	void SignalComponent
	::recieve(int id, bool state) {
		if(signal_ && (recieveMask_ & (1L << id)) != 0) {
			signal_->recieve(*this, id, state);
		}
	}


	void SignalComponent
	::areaChanged(SimComposite* newArea, SimComposite* oldArea) {
		if(oldArea && !sendState_ && sendId_ >= 0) {
			SignalAreaComponent* oAreaSignal = SignalAreaComponent::get(*oldArea);
			oAreaSignal->setSignalActive(sendId_, sendState_);
		}
		if(newArea && !sendState_ && sendId_ >= 0) {
			SignalAreaComponent* nAreaSignal = SignalAreaComponent::get(*newArea);
			nAreaSignal->setSignalActive(sendId_, sendState_);
		}

		if(newArea) {
			SimNodeComponent* n = static_cast<SimNodeComponent*>(newArea->component(type()));
			setParent(*n);
		}
		else {
			resetParent();
		}
	}

}
