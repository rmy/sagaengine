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


#include "ObjectList.hpp"
#include "comp/Object.hpp"
#include "comp/schema/CompSchema.hpp"
#include "util/error/Log.hpp"
#include <cstdio>


namespace se_core {

	ObjectList
	::ObjectList()
		: firstNode_(CompSchema::VoidList::end()) {
	}


	ObjectList
	::~ObjectList() {
		CompSchema::voidList.removeChain(firstNode_);
		firstNode_ = CompSchema::VoidList::end();
	}


	void ObjectList
	::add(const Object& value) {
		Assert(&value);
		CompSchema::voidList.add(&const_cast<Object&>(value), firstNode_);
	}


	void ObjectList
	::remove(const Object& value) {
		CompSchema::voidList.remove(&const_cast<Object&>(value), firstNode_);
	}


	bool ObjectList
	::contains(const Object& value) const {
		CompSchema::VoidList::iterator_type it = iterator();
		while(it != CompSchema::VoidList::end()) {
			if(CompSchema::voidList.next(it) == &value)
				return true;
		}
		return false;
	}


	bool ObjectList
	::contains(ObjectList& msc) const {
		CompSchema::VoidList::iterator_type it = msc.iterator();
		while(it != CompSchema::VoidList::end()) {
			if(!contains(*static_cast<Object*>(CompSchema::voidList.next(it))))
				return false;
		}
		return true;
	}


	bool ObjectList
	::sharesAny(ObjectList& msc) const {
		CompSchema::VoidList::iterator_type it = msc.iterator();
		while(it != CompSchema::VoidList::end()) {
			if(contains(*static_cast<Object*>(CompSchema::voidList.next(it)))) {
				return true;
			}
		}
		return false;
	}


	void ObjectList
	::add(ObjectList& msc) {
		Assert(&msc);
		CompSchema::VoidList::iterator_type it = msc.iterator();
		while(it != CompSchema::VoidList::end()) {
			add(*static_cast<Object*>(CompSchema::voidList.next(it)));
		}
	}


	void ObjectList
	::destroyMembersAndClear() {
		CompSchema::VoidList::iterator_type it = iterator();
		while(it != CompSchema::VoidList::end()) {
			delete static_cast<Object*>(CompSchema::voidList.next(it));
		}
		clear();
	}


	void ObjectList
	::initIterator(CompSchema::VoidList::iterator_type& iterator) const {
		iterator = firstNode_;
	}


	bool ObjectList
	::isEmpty() const {
		return (firstNode_ == CompSchema::VoidList::end());
	}


	void ObjectList
	::clear() {
		CompSchema::voidList.removeChain(firstNode_);
	}


	int ObjectList
	::size() const {
		return CompSchema::voidList.size(firstNode_);
	}



	ObjectList::Iterator
	::Iterator()
		: it_(CompSchema::VoidList::end()) {
	}


	ObjectList::Iterator
	::Iterator(const ObjectList& msc) {
		(*this).init(msc);
	}


	void ObjectList::Iterator
	::init(const ObjectList& msc) {
		msc.initIterator(it_);
	}


	void ObjectList::Iterator
	::init(short firstNode) {
		it_ = firstNode;
	}


}
