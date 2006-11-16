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


#include "MultiSimComposite.hpp"
#include "VoidList.hpp"
#include "../SimComposite.hpp"
#include "../schema/SimSchema.hpp"
#include "../../util/error/Log.hpp"
#include <cstdio>


namespace se_core {

	MultiSimComposite
	::MultiSimComposite()
		: firstNode_(VoidList::end()) {
	}


	MultiSimComposite
	::~MultiSimComposite() {
		SimSchema::voidList.removeChain(firstNode_);
		firstNode_ = VoidList::end();
	}


	void MultiSimComposite
	::add(SimComposite& value) {
		Assert(&value);
		SimSchema::voidList.add(&value, firstNode_);
	}


	void MultiSimComposite
	::remove(SimComposite& value) {
		SimSchema::voidList.remove(&value, firstNode_);
	}


	bool MultiSimComposite
	::contains(SimComposite& value) const {
		VoidList::iterator_type it = iterator();
		while(it != VoidList::end()) {
			if(SimSchema::voidList.next(it) == &value)
				return true;
		}
		return false;
	}


	bool MultiSimComposite
	::contains(MultiSimComposite& msc) const {
		VoidList::iterator_type it = msc.iterator();
		while(it != VoidList::end()) {
			if(!contains(*static_cast<SimComposite*>(SimSchema::voidList.next(it))))
				return false;
		}
		return true;
	}


	bool MultiSimComposite
	::sharesAny(MultiSimComposite& msc) const {
		VoidList::iterator_type it = msc.iterator();
		while(it != VoidList::end()) {
			if(contains(*static_cast<SimComposite*>(SimSchema::voidList.next(it)))) {
				return true;
			}
		}
		return false;
	}


	void MultiSimComposite
	::add(MultiSimComposite& msc) {
		Assert(&msc);
		VoidList::iterator_type it = msc.iterator();
		while(it != VoidList::end()) {
			add(*static_cast<SimComposite*>(SimSchema::voidList.next(it)));
		}
	}


	void MultiSimComposite
	::destroyMembersAndClear() {
		VoidList::iterator_type it = iterator();
		while(it != VoidList::end()) {
			delete static_cast<SimComposite*>(SimSchema::voidList.next(it));
		}
		clear();
	}


	void MultiSimComposite
	::initIterator(VoidList::iterator_type& iterator) const {
		iterator = firstNode_;
	}


	bool MultiSimComposite
	::isEmpty() const {
		return (firstNode_ == VoidList::end());
	}


	void MultiSimComposite
	::clear() {
		SimSchema::voidList.removeChain(firstNode_);
	}


	int MultiSimComposite
	::size() const {
		return SimSchema::voidList.size(firstNode_);
	}



	MultiSimComposite::Iterator
	::Iterator()
		: it_(VoidList::end()) {
	}


	MultiSimComposite::Iterator
	::Iterator(const MultiSimComposite& msc) {
		(*this).init(msc);
	}


	void MultiSimComposite::Iterator
	::init(const MultiSimComposite& msc) {
		msc.initIterator(it_);
	}


	void MultiSimComposite::Iterator
	::init(short firstNode) {
		it_ = firstNode;
	}


}
