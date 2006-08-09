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


#include "MultiSimObject.hpp"
#include "SimObjectList.hpp"
#include "SimObjectIterator.hpp"
#include "../SimObject.hpp"
#include "../schema/SimSchema.hpp"
#include "../error/sim_error.hpp"
#include "../../util/error/Log.hpp"
#include <cstdio>


namespace se_core {

	MultiSimObject
	::MultiSimObject()
		: firstNode_(SimObjectList::NULL_NODE) {
	}


	MultiSimObject
	::~MultiSimObject() {
		SimSchema::simObjectList.removeChain(firstNode_);
		firstNode_ = SimObjectList::NULL_NODE;
	}


	void MultiSimObject
	::add(SimObject& value) {
		Assert(&value);
		SimSchema::simObjectList.add(value, firstNode_);
	}


	void MultiSimObject
	::remove(SimObject& value) {
		SimSchema::simObjectList.remove(value, firstNode_);
	}


	bool MultiSimObject
	::contains(SimObject& value) const {
		SimObjectList::iterator_type it = iterator();
		while(it != SimObjectList::NULL_NODE) {
			if(SimSchema::simObjectList.next(it)->id() == value.id())
				return true;
		}
		return false;
	}


	bool MultiSimObject
	::contains(MultiSimObject& mgo) const {
		SimObjectList::iterator_type it = mgo.iterator();
		while(it != SimObjectList::NULL_NODE) {
			if(!contains(*SimSchema::simObjectList.next(it)))
				return false;
		}
		return true;
	}


	bool MultiSimObject
	::sharesAny(MultiSimObject& mgo) const {
		SimObjectList::iterator_type it = mgo.iterator();
		while(it != SimObjectList::NULL_NODE) {
			if(contains(*SimSchema::simObjectList.next(it))) {
				return true;
			}
		}
		return false;
	}


	void MultiSimObject
	::add(MultiSimObject& mgo) {
		Assert(&mgo);
		SimObjectList::iterator_type it = mgo.iterator();
		while(it != SimObjectList::NULL_NODE) {
			add(*SimSchema::simObjectList.next(it));
		}
	}


	void MultiSimObject
	::destroyMembersAndClear() {
		SimObjectList::iterator_type it = iterator();
		while(it != SimObjectList::NULL_NODE) {
			delete SimSchema::simObjectList.next(it);
		}
		clear();
	}


	void MultiSimObject
	::initIterator(SimObjectList::iterator_type& iterator) const {
		iterator = firstNode_;
	}


	bool MultiSimObject
	::isEmpty() const {
		return (firstNode_ == SimObjectList::NULL_NODE);
	}


	void MultiSimObject
	::clear() {
		SimSchema::simObjectList.removeChain(firstNode_);
	}


	int MultiSimObject
	::size() const {
		return SimSchema::simObjectList.size(firstNode_);
	}

}
