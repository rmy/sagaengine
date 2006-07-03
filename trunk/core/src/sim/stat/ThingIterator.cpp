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


#include "ThingIterator.hpp"
#include "MultiSimObjectIterator.hpp"
#include "../thing/Thing.hpp"

namespace se_core {
	ThingIterator::ThingIterator()
		: iterator(new MultiSimObjectIterator()) {
	}

	ThingIterator::~ThingIterator() {
		delete iterator;
	}


	ThingIterator::ThingIterator(MultiSimObject& mgo)
		: iterator(new MultiSimObjectIterator(mgo)) {
	}


	void
	ThingIterator::init(MultiSimObject& mgo) {
		(*this).iterator->init(mgo);
	}


	void
	ThingIterator::init(short& iterator) {
		(*this).iterator->init(iterator);
	}


	bool
	ThingIterator::hasNext() {
		return iterator->hasNext();
	}


	Thing&
	ThingIterator::next() {
		return static_cast<Thing&>(iterator->next());
	}

}
