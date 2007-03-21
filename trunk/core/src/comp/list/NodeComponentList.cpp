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


#include "NodeComponentList.hpp"
#include "comp/node/NodeComponent.hpp"
#include "util/error/Log.hpp"


namespace se_core {

	NodeComponentList
	::NodeComponentList()
		: firstNode_(CompSchema::VoidList::end()) {
	}


	NodeComponentList
	::~NodeComponentList() {
		CompSchema::voidList.removeChain(firstNode_);
		firstNode_ = CompSchema::VoidList::end();
	}


	void NodeComponentList
	::add(NodeComponent& value) {
		Assert(&value);
		CompSchema::voidList.add(&value, firstNode_);
	}


	void NodeComponentList
	::remove(NodeComponent& value) {
		CompSchema::voidList.remove(&value, firstNode_);
	}


	bool NodeComponentList
	::contains(NodeComponent& value) const {
		CompSchema::VoidList::iterator_type it = iterator();
		while(it != CompSchema::VoidList::end()) {
			if(CompSchema::voidList.next(it) == &value)
				return true;
		}
		return false;
	}


	bool NodeComponentList
	::contains(NodeComponentList& msc) const {
		CompSchema::VoidList::iterator_type it = msc.iterator();
		while(it != CompSchema::VoidList::end()) {
			if(!contains(*static_cast<NodeComponent*>(CompSchema::voidList.next(it))))
				return false;
		}
		return true;
	}


	bool NodeComponentList
	::sharesAny(NodeComponentList& msc) const {
		CompSchema::VoidList::iterator_type it = msc.iterator();
		while(it != CompSchema::VoidList::end()) {
			if(contains(*static_cast<NodeComponent*>(CompSchema::voidList.next(it)))) {
				return true;
			}
		}
		return false;
	}


	void NodeComponentList
	::add(NodeComponentList& msc) {
		Assert(&msc);
		CompSchema::VoidList::iterator_type it = msc.iterator();
		while(it != CompSchema::VoidList::end()) {
			add(*static_cast<NodeComponent*>(CompSchema::voidList.next(it)));
		}
	}


	void NodeComponentList
	::destroyMembersAndClear() {
		CompSchema::VoidList::iterator_type it = iterator();
		while(it != CompSchema::VoidList::end()) {
			delete static_cast<NodeComponent*>(CompSchema::voidList.next(it));
		}
		clear();
	}


	void NodeComponentList
	::initIterator(CompSchema::VoidList::iterator_type& iterator) const {
		iterator = firstNode_;
	}


	bool NodeComponentList
	::isEmpty() const {
		return (firstNode_ == CompSchema::VoidList::end());
	}


	void NodeComponentList
	::clear() {
		CompSchema::voidList.removeChain(firstNode_);
	}


	int NodeComponentList
	::size() const {
		return CompSchema::voidList.size(firstNode_);
	}


	NodeComponentList::Iterator
	::Iterator()
		: it_(CompSchema::VoidList::end()) {
	}


	NodeComponentList::Iterator
	::Iterator(NodeComponentList& msc) {
		(*this).init(msc);
	}


	void NodeComponentList::Iterator
	::init(NodeComponentList& msc) {
		msc.initIterator(it_);
	}


	void NodeComponentList::Iterator
	::init(short firstNode) {
		it_ = firstNode;
	}


	/// TreeIterator
	NodeComponentList::TreeIterator
	::TreeIterator()
		: stackPointer_(-1) {
	}


	NodeComponentList::TreeIterator
	::TreeIterator(NodeComponentList& msc) {
		if(msc.isEmpty()) {
			stackPointer_ = -1;
			return;
		}
		itStack_[ 0 ].init(msc);
		stackPointer_ = 0;
	}


	void NodeComponentList::TreeIterator
	::init(NodeComponentList& msc) {
		if(msc.isEmpty()) {
			stackPointer_ = -1;
			return;
		}
		itStack_[ 0 ].init(msc);
		stackPointer_ = 0;
	}


	bool NodeComponentList::TreeIterator
	::hasNext() {
		return (stackPointer_ >= 0);
	}


	NodeComponent& NodeComponentList::TreeIterator
	::next() {
		// Get next in chain
		NodeComponent& c = itStack_[ stackPointer_ ].next();

		// Stack overflowed?
		Assert(stackPointer_ < MAX_STACK_DEPTH - 1);

		// Push child chain as next chain on stack
		itStack_[ ++stackPointer_ ].init(c.children());

		// Pop all completed chains
		while(stackPointer_ >= 0 && !itStack_[ stackPointer_ ].hasNext()) {
			--stackPointer_;
		}

		return c;
	}


}
