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


#include "CompositeList.hpp"
#include "../Composite.hpp"
#include "../schema/CompSchema.hpp"
#include "util/error/Log.hpp"
#include <cstdio>


namespace se_core {

	CompositeList
	::CompositeList()
		: firstNode_(CompSchema::VoidList::end()) {
	}


	CompositeList
	::~CompositeList() {
		CompSchema::voidList.removeChain(firstNode_);
		firstNode_ = CompSchema::VoidList::end();
	}


	void CompositeList
	::add(Composite& value) {
		Assert(&value);
		CompSchema::voidList.add(&value, firstNode_);
	}


	void CompositeList
	::remove(Composite& value) {
		CompSchema::voidList.remove(&value, firstNode_);
	}


	bool CompositeList
	::contains(Composite& value) const {
		CompSchema::VoidList::iterator_type it = iterator();
		while(it != CompSchema::VoidList::end()) {
			if(CompSchema::voidList.next(it) == &value)
				return true;
		}
		return false;
	}


	bool CompositeList
	::contains(CompositeList& msc) const {
		CompSchema::VoidList::iterator_type it = msc.iterator();
		while(it != CompSchema::VoidList::end()) {
			if(!contains(*static_cast<Composite*>(CompSchema::voidList.next(it))))
				return false;
		}
		return true;
	}


	bool CompositeList
	::sharesAny(CompositeList& msc) const {
		CompSchema::VoidList::iterator_type it = msc.iterator();
		while(it != CompSchema::VoidList::end()) {
			if(contains(*static_cast<Composite*>(CompSchema::voidList.next(it)))) {
				return true;
			}
		}
		return false;
	}


	void CompositeList
	::add(CompositeList& msc) {
		Assert(&msc);
		CompSchema::VoidList::iterator_type it = msc.iterator();
		while(it != CompSchema::VoidList::end()) {
			add(*static_cast<Composite*>(CompSchema::voidList.next(it)));
		}
	}


	void CompositeList
	::destroyMembersAndClear() {
		CompSchema::VoidList::iterator_type it = iterator();
		while(it != CompSchema::VoidList::end()) {
			delete static_cast<Composite*>(CompSchema::voidList.next(it));
		}
		clear();
	}


	void CompositeList
	::initIterator(CompSchema::VoidList::iterator_type& iterator) const {
		iterator = firstNode_;
	}


	bool CompositeList
	::isEmpty() const {
		return (firstNode_ == CompSchema::VoidList::end());
	}


	void CompositeList
	::clear() {
		CompSchema::voidList.removeChain(firstNode_);
	}


	int CompositeList
	::size() const {
		return CompSchema::voidList.size(firstNode_);
	}



	CompositeList::Iterator
	::Iterator()
		: it_(CompSchema::VoidList::end()) {
	}


	CompositeList::Iterator
	::Iterator(const CompositeList& msc) {
		(*this).init(msc);
	}


	void CompositeList::Iterator
	::init(const CompositeList& msc) {
		msc.initIterator(it_);
	}


	void CompositeList::Iterator
	::init(short firstNode) {
		it_ = firstNode;
	}


}
