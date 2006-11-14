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


#include "MultiSimNodeComponent.hpp"
#include "VoidList.hpp"
#include "../SimNodeComponent.hpp"
#include "../schema/SimSchema.hpp"
#include "../../util/error/Log.hpp"
#include <cstdio>


namespace se_core {

	MultiSimNodeComponent
	::MultiSimNodeComponent()
		: firstNode_(VoidList::end()) {
	}


	MultiSimNodeComponent
	::~MultiSimNodeComponent() {
		SimSchema::voidList.removeChain(firstNode_);
		firstNode_ = VoidList::end();
	}


	void MultiSimNodeComponent
	::add(SimNodeComponent& value) {
		Assert(&value);
		SimSchema::voidList.add(&value, firstNode_);
	}


	void MultiSimNodeComponent
	::remove(SimNodeComponent& value) {
		SimSchema::voidList.remove(&value, firstNode_);
	}


	bool MultiSimNodeComponent
	::contains(SimNodeComponent& value) const {
		VoidList::iterator_type it = iterator();
		while(it != VoidList::end()) {
			if(SimSchema::voidList.next(it) == &value)
				return true;
		}
		return false;
	}


	bool MultiSimNodeComponent
	::contains(MultiSimNodeComponent& msc) const {
		VoidList::iterator_type it = msc.iterator();
		while(it != VoidList::end()) {
			if(!contains(*static_cast<SimNodeComponent*>(SimSchema::voidList.next(it))))
				return false;
		}
		return true;
	}


	bool MultiSimNodeComponent
	::sharesAny(MultiSimNodeComponent& msc) const {
		VoidList::iterator_type it = msc.iterator();
		while(it != VoidList::end()) {
			if(contains(*static_cast<SimNodeComponent*>(SimSchema::voidList.next(it)))) {
				return true;
			}
		}
		return false;
	}


	void MultiSimNodeComponent
	::add(MultiSimNodeComponent& msc) {
		Assert(&msc);
		VoidList::iterator_type it = msc.iterator();
		while(it != VoidList::end()) {
			add(*static_cast<SimNodeComponent*>(SimSchema::voidList.next(it)));
		}
	}


	void MultiSimNodeComponent
	::destroyMembersAndClear() {
		VoidList::iterator_type it = iterator();
		while(it != VoidList::end()) {
			delete static_cast<SimNodeComponent*>(SimSchema::voidList.next(it));
		}
		clear();
	}


	void MultiSimNodeComponent
	::initIterator(VoidList::iterator_type& iterator) const {
		iterator = firstNode_;
	}


	bool MultiSimNodeComponent
	::isEmpty() const {
		return (firstNode_ == VoidList::end());
	}


	void MultiSimNodeComponent
	::clear() {
		SimSchema::voidList.removeChain(firstNode_);
	}


	int MultiSimNodeComponent
	::size() const {
		return SimSchema::voidList.size(firstNode_);
	}


	MultiSimNodeComponent::Iterator
	::Iterator()
		: it_(VoidList::end()) {
	}


	MultiSimNodeComponent::Iterator
	::Iterator(MultiSimNodeComponent& msc) {
		(*this).init(msc);
	}


	void MultiSimNodeComponent::Iterator
	::init(MultiSimNodeComponent& msc) {
		msc.initIterator(it_);
	}


	void MultiSimNodeComponent::Iterator
	::init(short firstNode) {
		it_ = firstNode;
	}


	/// TreeIterator
	MultiSimNodeComponent::TreeIterator
	::TreeIterator()
		: stackPointer_(-1) {
	}


	MultiSimNodeComponent::TreeIterator
	::TreeIterator(MultiSimNodeComponent& msc) {
		if(msc.isEmpty()) {
			stackPointer_ = -1;
			return;
		}
		itStack_[ 0 ].init(msc);
		stackPointer_ = 0;
	}


	void MultiSimNodeComponent::TreeIterator
	::init(MultiSimNodeComponent& msc) {
		if(msc.isEmpty()) {
			stackPointer_ = -1;
			return;
		}
		itStack_[ 0 ].init(msc);
		stackPointer_ = 0;
	}


	bool MultiSimNodeComponent::TreeIterator
	::hasNext() {
		return (stackPointer_ >= 0);
	}


	SimNodeComponent& MultiSimNodeComponent::TreeIterator
	::next() {
		// Get next in chain
		SimNodeComponent& c = itStack_[ stackPointer_ ].next();

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
