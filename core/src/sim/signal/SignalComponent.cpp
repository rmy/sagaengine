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
#include "../action/all.hpp"
#include "../thing/Actor.hpp"
#include "../schema/SimSchema.hpp"
#include "../stat/SortedSimObjectList.hpp"
#include "util/type/String.hpp"
#include "util/error/Log.hpp"


namespace se_core {
	SignalComponent
	::SignalComponent(SimComposite* owner)
		: AreaChildComponent(sct_SIGNAL, owner), sendState_(false), sendId_(1), recieveId_(1)
		, signal_(0) {
	}


	SignalComponent
	::~SignalComponent() {
		cleanup();
	}


	void SignalComponent
	::cleanup() {
	}


	void SignalComponent
	::send(bool state) {
		if(sendState_ == state)
			return;
		sendState_ = state;
	}


	void SignalComponent
	::recieve(int id, bool state) {
		if(recieveId_ == id) {
			signal_->recieve(id, state);
		}
	}

}
