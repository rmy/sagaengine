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


#include "ThingMOInfo.hpp"
#include "ThingMOManager.hpp"
#include "ThingMultiMO.hpp"
#include "../schema/O3dSchema.hpp"
#include <cstdio>
#include <cstring>

using namespace se_core;

namespace se_ogre {

	ThingMultiMO
	::ThingMultiMO(se_core::PosComponent& thing, const ThingMOInfo& info, const ThingMOFactory& factory)
		: ThingMO(thing, info, factory) {
		hasAnimation_ = true;
	}


	ThingMultiMO
	::~ThingMultiMO() {
		//ThingMOList::iterator_type it = firstThingMO_;
		//while(it != CompSchema::VoidList::end()) {
		while(!moList_.isEmpty()) {
			ThingMO* te = &moList_.pop(); // O3dSchema::thingMOList.next(it);
			O3dSchema::thingMOManager.release(te);
		}
	}


	void ThingMultiMO
	::moveChildren(long when, float stepDelta, float timeSinceLastFrame) {
		//ThingMOList::iterator_type it = firstThingMO_;
		//while(it != CompSchema::VoidList::end()) {
		ThingMOList::Iterator it(moList_);
		while(it.hasNext()) {
			ThingMO* te = &it.next(); // O3dSchema::thingMOList.next(it);
			te->move(when, stepDelta, timeSinceLastFrame);
			te->resetPos();
		}
	}


	void ThingMultiMO
	::animateChildren(long when, float stepDelta, float timeSinceLastFrame) {
		moveChildren(when, stepDelta, timeSinceLastFrame);
		//ThingMOList::iterator_type it = firstThingMO_;
		//while(it != CompSchema::VoidList::end()) {
		//	ThingMO* te = O3dSchema::thingMOList.next(it);
		ThingMOList::Iterator it(moList_);
		while(it.hasNext()) {
			ThingMO* te = &it.next(); // O3dSchema::thingMOList.next(it);
			if(te->hasAnimation()) {
				te->animate(when, stepDelta, timeSinceLastFrame);
			}
		}
	}


	void ThingMultiMO
	::add(ThingMO& tmo) {
		moList_.add(tmo);
		tmo.setParentNode(node_);
		tmo.setVisible(true);
		//if(tmo.hasAnimation())
		//	hasAnimation_ = true;
	}


	void ThingMultiMO
	::remove(ThingMO& tmo) {
		moList_.remove(tmo);
	}

}
