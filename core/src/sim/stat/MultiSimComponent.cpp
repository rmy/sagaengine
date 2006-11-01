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
#include "SimComponentList.hpp"
#include "../SimComponent.hpp"
#include "../schema/SimSchema.hpp"
#include "../../util/error/Log.hpp"
#include <cstdio>


namespace se_core {

	MultiSimComponent
	::MultiSimComponent()
		: firstNode_(SimComponentList::end()) {
	}


	MultiSimComponent
	::~MultiSimComponent() {
		SimSchema::simComponentList.removeChain(firstNode_);
		firstNode_ = SimComponentList::end();
	}


	void MultiSimComponent
	::add(SimComponent& value) {
		Assert(&value);
		SimSchema::simComponentList.add(value, firstNode_);
	}


	void MultiSimComponent
	::remove(SimComponent& value) {
		SimSchema::simComponentList.remove(value, firstNode_);
	}


	bool MultiSimComponent
	::contains(SimComponent& value) const {
		SimComponentList::iterator_type it = iterator();
		while(it != SimComponentList::end()) {
			if(SimSchema::simComponentList.next(it) == &value)
				return true;
		}
		return false;
	}


	bool MultiSimComponent
	::contains(MultiSimComponent& msc) const {
		SimComponentList::iterator_type it = msc.iterator();
		while(it != SimComponentList::end()) {
			if(!contains(*SimSchema::simComponentList.next(it)))
				return false;
		}
		return true;
	}


	bool MultiSimComponent
	::sharesAny(MultiSimComponent& msc) const {
		SimComponentList::iterator_type it = msc.iterator();
		while(it != SimComponentList::end()) {
			if(contains(*SimSchema::simComponentList.next(it))) {
				return true;
			}
		}
		return false;
	}


	void MultiSimComponent
	::add(MultiSimComponent& msc) {
		Assert(&msc);
		SimComponentList::iterator_type it = msc.iterator();
		while(it != SimComponentList::end()) {
			add(*SimSchema::simComponentList.next(it));
		}
	}


	void MultiSimComponent
	::destroyMembersAndClear() {
		SimComponentList::iterator_type it = iterator();
		while(it != SimComponentList::end()) {
			delete SimSchema::simComponentList.next(it);
		}
		clear();
	}


	void MultiSimComponent
	::initIterator(SimComponentList::iterator_type& iterator) const {
		iterator = firstNode_;
	}


	bool MultiSimComponent
	::isEmpty() const {
		return (firstNode_ == SimComponentList::end());
	}


	void MultiSimComponent
	::clear() {
		SimSchema::simComponentList.removeChain(firstNode_);
	}


	int MultiSimComponent
	::size() const {
		return SimSchema::simComponentList.size(firstNode_);
	}

}
