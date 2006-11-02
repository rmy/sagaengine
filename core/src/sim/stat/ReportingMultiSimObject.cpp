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


#include "ReportingMultiSimObject.hpp"
#include "SimObjectList.hpp"
#include "MultiSimObjectListener.hpp"
#include "../SimObject.hpp"
#include "../schema/SimSchema.hpp"

namespace se_core {


	ReportingMultiSimObject
	::ReportingMultiSimObject(SimObject& owner, short type)
		: type_(type), owner_(&owner), handler_(0) {
	}


	ReportingMultiSimObject
	::ReportingMultiSimObject(short type)
		: type_(type), handler_(0) {
	}


	ReportingMultiSimObject
	::~ReportingMultiSimObject() {
	}


	void ReportingMultiSimObject
	::add(SimObject& value) {
		SimSchema::simObjectList.add(&value, firstNode_);
		if(handler_) handler_->simObjectAddedEvent(*this, value);
		//if(owner_) SimSchema::multiSimObjectListeners.castSimObjectAddedEvent(*this, value);
	}


	void ReportingMultiSimObject
	::remove(SimObject& value) {
		SimSchema::simObjectList.remove(&value, firstNode_);
		if(handler_) handler_->simObjectRemovedEvent(*this, value);
		//if(owner_) SimSchema::multiSimObjectListeners.castSimObjectRemovedEvent(*this, value);
	}

}
