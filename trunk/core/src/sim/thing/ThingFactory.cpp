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


#include "ThingFactory.hpp"
#include "ThingManager.hpp"
#include "Thing.hpp"
#include "../config/all.hpp"
#include "../stat/all.hpp"
#include "util/type/all.hpp"
#include "util/error/Log.hpp"
#include <cstring>
#include <cstdio>

namespace se_core {
	/*
	ThingFactory
	::ThingFactory(short type, String* name)
			: name_(name), type_(type), singleValues_(0), multiSimObjects_(0), attributes_(0) {
	}


	ThingFactory
	::~ThingFactory() {
		delete name_;
	}


	void ThingFactory
	::release(Thing* t) {
		delete t;
	}


	const char* ThingFactory
	::name() const {
		return name_->get();
	}


	MultiSimObject& ThingFactory
	::multiSimObject(int type) {
		Assert(multiSimObjects_ && "Subclass must create multiSimObjects_ array of right size");
		if(multiSimObjects_[type] == 0) {
			multiSimObjects_[type] = new MultiSimObject();
		}

		return *multiSimObjects_[type];
	}


	void ThingFactory
	::setSingleValue(short id, short value) {
		Assert(multiSimObjects_ && "Subclass must create singleValues_ array of right size");
		singleValues_[id] = value;
	}


	String& ThingFactory
	::attribute(short type) {
		Assert(multiSimObjects_ && "Subclass must create attributes_ array of right size");
		return attributes_[type];
	}

	*/
}
