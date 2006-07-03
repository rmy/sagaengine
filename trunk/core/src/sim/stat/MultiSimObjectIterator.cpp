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


#include "MultiSimObjectIterator.hpp"
#include "MultiSimObject.hpp"

#include "SimObjectList.hpp"
#include "../schema/SimSchema.hpp"
#include "../thing/Thing.hpp"
#include "../thing/Actor.hpp"
#include "../script/Cutscene.hpp"
#include "../script/QuestGoal.hpp"


namespace se_core {


	MultiSimObjectIterator
	::MultiSimObjectIterator()
		: it_(SimObjectList::NULL_NODE) {
	}


	MultiSimObjectIterator
	::MultiSimObjectIterator(MultiSimObject& mgo) {
		(*this).init(mgo);
	}

	void MultiSimObjectIterator
	::init(MultiSimObject& mgo) {
		mgo.initIterator(it_);
	}


	void MultiSimObjectIterator
	::init(short firstNode) {
		it_ = firstNode;
	}

	bool MultiSimObjectIterator
	::hasNext() {
		return it_ != SimObjectList::NULL_NODE;
	}

	SimObject& MultiSimObjectIterator
	::next() {
		return *(SimSchema::simObjectList.next(it_));
	}


	Thing& MultiSimObjectIterator
	::nextThing() {
		return static_cast<Thing&>(*SimSchema::simObjectList.next(it_));
	}


	Actor& MultiSimObjectIterator
	::nextActor() {
		return static_cast<Actor&>(*SimSchema::simObjectList.next(it_));
	}


	Cutscene& MultiSimObjectIterator
	::nextCutscene() {
		return static_cast<Cutscene&>(*SimSchema::simObjectList.next(it_));
	}


	QuestGoal& MultiSimObjectIterator
	::nextQuestGoal() {
		return static_cast<QuestGoal&>(*SimSchema::simObjectList.next(it_));
	}

}
