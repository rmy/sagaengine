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


#include "MultiSimComponent.hpp"
#include "VoidList.hpp"
#include "../SimComponent.hpp"
#include "../schema/SimSchema.hpp"
#include "../../util/error/Log.hpp"
#include <cstdio>


namespace se_core {

	MultiSimComponent
	::MultiSimComponent()
		: firstNode_(VoidList::end()) {
	}


	MultiSimComponent
	::~MultiSimComponent() {
		SimSchema::voidList.removeChain(firstNode_);
		firstNode_ = VoidList::end();
	}


	void MultiSimComponent
	::add(SimComponent& value) {
		Assert(&value);
		SimSchema::voidList.add(&value, firstNode_);
	}


	void MultiSimComponent
	::remove(SimComponent& value) {
		SimSchema::voidList.remove(&value, firstNode_);
	}


	bool MultiSimComponent
	::contains(SimComponent& value) const {
		VoidList::iterator_type it = iterator();
		while(it != VoidList::end()) {
			if(SimSchema::voidList.next(it) == &value)
				return true;
		}
		return false;
	}


	bool MultiSimComponent
	::contains(MultiSimComponent& msc) const {
		VoidList::iterator_type it = msc.iterator();
		while(it != VoidList::end()) {
			if(!contains(*static_cast<SimComponent*>(SimSchema::voidList.next(it))))
				return false;
		}
		return true;
	}


	bool MultiSimComponent
	::sharesAny(MultiSimComponent& msc) const {
		VoidList::iterator_type it = msc.iterator();
		while(it != VoidList::end()) {
			if(contains(*static_cast<SimComponent*>(SimSchema::voidList.next(it)))) {
				return true;
			}
		}
		return false;
	}


	void MultiSimComponent
	::add(MultiSimComponent& msc) {
		Assert(&msc);
		VoidList::iterator_type it = msc.iterator();
		while(it != VoidList::end()) {
			add(*static_cast<SimComponent*>(SimSchema::voidList.next(it)));
		}
	}


	void MultiSimComponent
	::destroyMembersAndClear() {
		VoidList::iterator_type it = iterator();
		while(it != VoidList::end()) {
			delete static_cast<SimComponent*>(SimSchema::voidList.next(it));
		}
		clear();
	}


	void MultiSimComponent
	::initIterator(VoidList::iterator_type& iterator) const {
		iterator = firstNode_;
	}


	bool MultiSimComponent
	::isEmpty() const {
		return (firstNode_ == VoidList::end());
	}


	void MultiSimComponent
	::clear() {
		SimSchema::voidList.removeChain(firstNode_);
	}


	int MultiSimComponent
	::size() const {
		return SimSchema::voidList.size(firstNode_);
	}



	MultiSimComponent::Iterator
	::Iterator()
		: it_(VoidList::end()) {
	}


	MultiSimComponent::Iterator
	::Iterator(const MultiSimComponent& msc) {
		(*this).init(msc);
	}


	void MultiSimComponent::Iterator
	::init(const MultiSimComponent& msc) {
		msc.initIterator(it_);
	}


	void MultiSimComponent::Iterator
	::init(short firstNode) {
		it_ = firstNode;
	}


}
