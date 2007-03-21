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


#include "ComponentList.hpp"
#include "comp/Component.hpp"
#include "../schema/CompSchema.hpp"
#include "util/error/Log.hpp"
#include <cstdio>


namespace se_core {

	ComponentList
	::ComponentList()
		: firstNode_(CompSchema::VoidList::end()) {
	}


	ComponentList
	::~ComponentList() {
		CompSchema::voidList.removeChain(firstNode_);
		firstNode_ = CompSchema::VoidList::end();
	}


	void ComponentList
	::add(Component& value) {
		Assert(&value);
		CompSchema::voidList.add(&value, firstNode_);
	}


	void ComponentList
	::remove(Component& value) {
		CompSchema::voidList.remove(&value, firstNode_);
	}


	bool ComponentList
	::contains(Component& value) const {
		CompSchema::VoidList::iterator_type it = iterator();
		while(it != CompSchema::VoidList::end()) {
			if(CompSchema::voidList.next(it) == &value)
				return true;
		}
		return false;
	}


	bool ComponentList
	::contains(ComponentList& msc) const {
		CompSchema::VoidList::iterator_type it = msc.iterator();
		while(it != CompSchema::VoidList::end()) {
			if(!contains(*static_cast<Component*>(CompSchema::voidList.next(it))))
				return false;
		}
		return true;
	}


	bool ComponentList
	::sharesAny(ComponentList& msc) const {
		CompSchema::VoidList::iterator_type it = msc.iterator();
		while(it != CompSchema::VoidList::end()) {
			if(contains(*static_cast<Component*>(CompSchema::voidList.next(it)))) {
				return true;
			}
		}
		return false;
	}


	void ComponentList
	::add(ComponentList& msc) {
		Assert(&msc);
		CompSchema::VoidList::iterator_type it = msc.iterator();
		while(it != CompSchema::VoidList::end()) {
			add(*static_cast<Component*>(CompSchema::voidList.next(it)));
		}
	}


	void ComponentList
	::destroyMembersAndClear() {
		CompSchema::VoidList::iterator_type it = iterator();
		while(it != CompSchema::VoidList::end()) {
			delete static_cast<Component*>(CompSchema::voidList.next(it));
		}
		clear();
	}


	void ComponentList
	::initIterator(CompSchema::VoidList::iterator_type& iterator) const {
		iterator = firstNode_;
	}


	bool ComponentList
	::isEmpty() const {
		return (firstNode_ == CompSchema::VoidList::end());
	}


	void ComponentList
	::clear() {
		CompSchema::voidList.removeChain(firstNode_);
	}


	int ComponentList
	::size() const {
		return CompSchema::voidList.size(firstNode_);
	}



	ComponentList::Iterator
	::Iterator()
		: it_(CompSchema::VoidList::end()) {
	}


	ComponentList::Iterator
	::Iterator(const ComponentList& msc) {
		(*this).init(msc);
	}


	void ComponentList::Iterator
	::init(const ComponentList& msc) {
		msc.initIterator(it_);
	}


	void ComponentList::Iterator
	::init(short firstNode) {
		it_ = firstNode;
	}


}
