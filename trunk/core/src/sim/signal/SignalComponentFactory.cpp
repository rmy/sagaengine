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


#include "SignalComponentFactory.hpp"
#include "SignalComponent.hpp"
#include "Signal.hpp"
#include "../sim.hpp"
#include "../SimEngine.hpp"
#include "../config/all.hpp"
#include "../stat/all.hpp"
#include "../schema/SimSchema.hpp"


namespace se_core {

	SignalComponentFactory
	::SignalComponentFactory()
		: SimComponentFactory(sct_SIGNAL), signalSendId_(0), signalRecieveMask_(0), signal_(0) {
	}


	void SignalComponentFactory
	::setSendSignal(int id) {
		signalSendId_ = id;
	}


	void SignalComponentFactory
	::setRecieveSignal(unsigned long mask, const char* signal) {
		signalRecieveMask_ = mask;
		signal_ = SimSchema::sortedSimObjectList().signal(signal);

	}


	Component* SignalComponentFactory
	::create(Composite* owner) const {
		// Try to get existing component 
		// - allows overrides of default values
		// (Useful when loading saved games).
		SignalComponent::Ptr pSignal(*owner);
		if(pSignal == 0) {
			// Create new component
			pSignal = new SignalComponent(owner, this);
		}

		if(signalSendId_) {
			pSignal->setSendId(signalSendId_);
		}
		if(signal_) {
			pSignal->setRecieveId(signalRecieveMask_, signal_);
		}

		return pSignal;
	}

}
