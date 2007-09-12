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


#include "ThingMOList.hpp"
#include "ThingMO.hpp"
#include "comp/schema/CompSchema.hpp"
#include "util/error/Log.hpp"
#include <cstdio>

using namespace se_core;

namespace se_ogre {

	ThingMOList
	::ThingMOList()
		: firstNode_(CompSchema::VoidList::end()) {
	}


	ThingMOList
	::~ThingMOList() {
		CompSchema::voidList.removeChain(firstNode_);
		firstNode_ = CompSchema::VoidList::end();
	}


	void ThingMOList
	::add(ThingMO& value) {
		Assert(&value);
		//LogWarning(value.name() << " added.");
		CompSchema::voidList.add(&const_cast<ThingMO&>(value), firstNode_);
	}


	void ThingMOList
	::remove(ThingMO& value) {
		//LogWarning(value.name() << " removed.");
		CompSchema::voidList.remove(&const_cast<ThingMO&>(value), firstNode_);
	}


	ThingMO& ThingMOList
	::pop() {
		Assert(firstNode_ != -1);
		ThingMO& value = *static_cast<ThingMO*>(CompSchema::voidList.pop(firstNode_));
		//LogWarning(value.name() << " popped.");
		return value;
	}


	bool ThingMOList
	::contains(const ThingMO& value) const {
		CompSchema::VoidList::iterator_type it = iterator();
		while(it != CompSchema::VoidList::end()) {
			if(CompSchema::voidList.next(it) == &value)
				return true;
		}
		return false;
	}


	bool ThingMOList
	::contains(ThingMOList& msc) const {
		CompSchema::VoidList::iterator_type it = msc.iterator();
		while(it != CompSchema::VoidList::end()) {
			if(!contains(*static_cast<ThingMO*>(CompSchema::voidList.next(it))))
				return false;
		}
		return true;
	}


	bool ThingMOList
	::sharesAny(ThingMOList& msc) const {
		CompSchema::VoidList::iterator_type it = msc.iterator();
		while(it != CompSchema::VoidList::end()) {
			if(contains(*static_cast<ThingMO*>(CompSchema::voidList.next(it)))) {
				return true;
			}
		}
		return false;
	}


	void ThingMOList
	::add(ThingMOList& msc) {
		Assert(&msc);
		CompSchema::VoidList::iterator_type it = msc.iterator();
		while(it != CompSchema::VoidList::end()) {
			add(*static_cast<ThingMO*>(CompSchema::voidList.next(it)));
		}
	}


	void ThingMOList
	::initIterator(CompSchema::VoidList::iterator_type& iterator) const {
		iterator = firstNode_;
	}


	bool ThingMOList
	::isEmpty() const {
		return (firstNode_ == CompSchema::VoidList::end());
	}


	void ThingMOList
	::clear() {
		CompSchema::voidList.removeChain(firstNode_);
	}


	int ThingMOList
	::size() const {
		return CompSchema::voidList.size(firstNode_);
	}



	ThingMOList::Iterator
	::Iterator()
		: it_(CompSchema::VoidList::end()) {
	}


	ThingMOList::Iterator
	::Iterator(const ThingMOList& msc) {
		(*this).init(msc);
	}


	void ThingMOList::Iterator
	::init(const ThingMOList& msc) {
		msc.initIterator(it_);
	}


	void ThingMOList::Iterator
	::init(short firstNode) {
		it_ = firstNode;
	}


}
