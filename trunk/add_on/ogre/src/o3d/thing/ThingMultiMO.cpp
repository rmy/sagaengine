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

namespace se_ogre {

	ThingMultiMO
	::ThingMultiMO(se_core::PosNode& thing, const ThingMOInfo& info, const ThingMOFactory& factory)
		: ThingMO(thing, info, factory), firstThingMO_(ThingMOList::end()) {
		hasAnimation_ = true;
	}


	ThingMultiMO
	::~ThingMultiMO() {
		ThingMOList::iterator_type it = firstThingMO_;
		while(it != ThingMOList::end()) {
			ThingMO* te = O3dSchema::thingMOList.next(it);
			O3dSchema::thingMOManager.release(te);
		}
	}


	void ThingMultiMO
	::moveChildren(long when, float stepDelta, float timeSinceLastFrame) {
		ThingMOList::iterator_type it = firstThingMO_;
		while(it != ThingMOList::end()) {
			ThingMO* te = O3dSchema::thingMOList.next(it);
			te->move(when, stepDelta, timeSinceLastFrame);
			te->resetPos();
		}
	}


	void ThingMultiMO
	::animateChildren(long when, float stepDelta, float timeSinceLastFrame) {
		moveChildren(when, stepDelta, timeSinceLastFrame);
		ThingMOList::iterator_type it = firstThingMO_;
		while(it != ThingMOList::end()) {
			ThingMO* te = O3dSchema::thingMOList.next(it);
			if(te->hasAnimation()) {
				te->animate(when, stepDelta, timeSinceLastFrame);
			}
		}
	}


	void ThingMultiMO
	::add(ThingMO& tmo) {
		O3dSchema::thingMOList.add(&tmo, firstThingMO_);
		tmo.setParentNode(node_);
		tmo.setVisible(true);
		//if(tmo.hasAnimation())
		//	hasAnimation_ = true;
	}


	void ThingMultiMO
	::remove(ThingMO& tmo) {
		O3dSchema::thingMOList.remove(&tmo, firstThingMO_);
	}

}
