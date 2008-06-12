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
#include "util/type/String.hpp"
#include "util/error/Log.hpp"


namespace se_core {
	SignalComponent
	::SignalComponent(Composite* owner, const ComponentFactory* factory)
		: AreaChildComponent(sct_SIGNAL, owner), sendState_(false), sendId_(-1), sentWhen_(0), recieveMask_(0)
		, signal_(0) {
	}


	SignalComponent
	::~SignalComponent() {
		cleanup();
	}


	void SignalComponent
	::cleanup() {
		signal_ = 0;
		sendId_ = -1;
		recieveMask_ = 0;
	}

	void SignalComponent
	::setDead() {
		LogDetail("Set dead with signal id: " << sendId_ << " - " << owner()->name());
		if(sendId_ >= 0)
			send(true);
	}

	bool SignalComponent
	::send(bool state) {
		sentWhen_ = SimSchema::simEngine.when();
		if(sendState_ == state)
			return false;
		sendState_ = state;

		if(parent()) {
			SignalAreaComponent* areaSignal = SignalAreaComponent::get(*parent());
			LogDetail("Sending signal state: " << sendId_ << " - " << sendState_ << ": " << owner()->name());
			areaSignal->setSignalActive(sendId_, sendState_);
		}
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
	::zoneChanged(int zoneType, Composite* newArea, Composite* oldArea) {
		if(zoneType != st_AREA)
			return;

		if(oldArea && !sendState_ && sendId_ >= 0) {
			SignalAreaComponent::Ptr oAreaSignal(*oldArea);
			oAreaSignal->setSignalActive(sendId_, sendState_);
			LogDetail("Leave area signal state: " << sendId_ << " - " << sendState_ << ": " << owner()->name());
		}
		if(newArea && !sendState_ && sendId_ >= 0) {
			SignalAreaComponent::Ptr nAreaSignal(*newArea);
			nAreaSignal->setSignalActive(sendId_, sendState_);
			LogDetail("Enter area signal state: " << sendId_ << " - " << sendState_ << ": " << owner()->name());
		}

		if(newArea) {
			NodeComponent* n = static_cast<NodeComponent*>(newArea->component(type()));
			setParent(*n);
		}
		else {
			resetParent();
		}
	}

}
