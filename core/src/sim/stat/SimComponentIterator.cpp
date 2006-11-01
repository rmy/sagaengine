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


#include "SimComponentIterator.hpp"
#include "MultiSimComponent.hpp"
#include "SimComponentList.hpp"
#include "../schema/SimSchema.hpp"
#include "../thing/Thing.hpp"
#include "../thing/Actor.hpp"
#include "../script/Cutscene.hpp"
#include "../script/QuestGoal.hpp"


namespace se_core {

	SimComponentIterator
	::SimComponentIterator()
		: it_(SimComponentList::end()) {
	}


	SimComponentIterator
	::SimComponentIterator(MultiSimComponent& msc) {
		(*this).init(msc);
	}


	void SimComponentIterator
	::init(MultiSimComponent& msc) {
		msc.initIterator(it_);
	}


	void SimComponentIterator
	::init(short firstNode) {
		it_ = firstNode;
	}

}
