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


#include "MultiO3dThingComponent.hpp"
#include <cstdio>

using namespace se_core;

namespace se_ogre {

	MultiO3dThingComponent
	::MultiO3dThingComponent()
		: firstNode_(VoidList::end()) {
	}


	MultiO3dThingComponent
	::~MultiO3dThingComponent() {
		SimSchema::voidList.removeChain(firstNode_);
		firstNode_ = VoidList::end();
	}


	void MultiO3dThingComponent
	::add(O3dThingComponent& value) {
		Assert(&value);
		SimSchema::voidList.add(&value, firstNode_);
	}


	void MultiO3dThingComponent
	::remove(O3dThingComponent& value) {
		SimSchema::voidList.remove(&value, firstNode_);
	}


	bool MultiO3dThingComponent
	::contains(O3dThingComponent& value) const {
		VoidList::iterator_type it = iterator();
		while(it != VoidList::end()) {
			if(SimSchema::voidList.next(it) == &value)
				return true;
		}
		return false;
	}


	bool MultiO3dThingComponent
	::contains(MultiO3dThingComponent& msc) const {
		VoidList::iterator_type it = msc.iterator();
		while(it != VoidList::end()) {
			if(!contains(*static_cast<O3dThingComponent*>(SimSchema::voidList.next(it))))
				return false;
		}
		return true;
	}


	bool MultiO3dThingComponent
	::sharesAny(MultiO3dThingComponent& msc) const {
		VoidList::iterator_type it = msc.iterator();
		while(it != VoidList::end()) {
			if(contains(*static_cast<O3dThingComponent*>(SimSchema::voidList.next(it)))) {
				return true;
			}
		}
		return false;
	}


	void MultiO3dThingComponent
	::add(MultiO3dThingComponent& msc) {
		Assert(&msc);
		VoidList::iterator_type it = msc.iterator();
		while(it != VoidList::end()) {
			add(*static_cast<O3dThingComponent*>(SimSchema::voidList.next(it)));
		}
	}


	void MultiO3dThingComponent
	::destroyMembersAndClear() {
		VoidList::iterator_type it = iterator();
		while(it != VoidList::end()) {
			delete static_cast<O3dThingComponent*>(SimSchema::voidList.next(it));
		}
		clear();
	}


	void MultiO3dThingComponent
	::initIterator(VoidList::iterator_type& iterator) const {
		iterator = firstNode_;
	}


	bool MultiO3dThingComponent
	::isEmpty() const {
		return (firstNode_ == VoidList::end());
	}


	void MultiO3dThingComponent
	::clear() {
		SimSchema::voidList.removeChain(firstNode_);
	}


	int MultiO3dThingComponent
	::size() const {
		return SimSchema::voidList.size(firstNode_);
	}



	MultiO3dThingComponent::Iterator
	::Iterator()
		: it_(VoidList::end()) {
	}


	MultiO3dThingComponent::Iterator
	::Iterator(MultiO3dThingComponent& msc) {
		(*this).init(msc);
	}


	void MultiO3dThingComponent::Iterator
	::init(MultiO3dThingComponent& msc) {
		msc.initIterator(it_);
	}


	void MultiO3dThingComponent::Iterator
	::init(short firstNode) {
		it_ = firstNode;
	}


}
