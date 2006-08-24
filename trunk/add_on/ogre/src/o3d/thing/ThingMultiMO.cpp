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
#include "ThingMultiMO.hpp"
#include "../schema/O3dSchema.hpp"
#include <cstdio>
#include <cstring>

namespace se_ogre {

	ThingMultiMO
	::ThingMultiMO(se_core::PosNode& thing, const ThingMOInfo& info, const ThingMOFactory& factory)
		: ThingMO(thing, info, factory) {

		// Create a unique entity name
		char name[128];
		sprintf(name, "%d-%s", thing_.id(), thing_.name());

		// Set the mesh
		movableObject_ = O3dSchema::sceneManager->createMovableObject(name, info_.movableObjectType_.get(), &info_.params_);
		node_->attachObject(movableObject_);
	}


	ThingMultiMO
	::~ThingMultiMO() {
		O3dSchema::sceneManager->destroyMovableObject(movableObject_);
		movableObject_ = 0;
	}


	void ThingMultiMO
	::moveChildren(long when, float stepDelta, float timeSinceLastFrame) {
		ThingMOList::iterator_type it = firstThingMO_;
		while(it != ThingMOList::NULL_NODE) {
			ThingMO* te = O3dSchema::thingMOList.next(it);
			te->move(when, stepDelta, timeSinceLastFrame);
		}
	}

	void ThingMultiMO
	::animateChildren(long when, float stepDelta, float timeSinceLastFrame) {
		ThingMOList::iterator_type it = firstThingMO_;
		while(it != ThingMOList::NULL_NODE) {
			ThingMO* te = O3dSchema::thingMOList.next(it);
			if(te->hasAnimation())
				te->animate(when, stepDelta, timeSinceLastFrame);
		}
	}


	void ThingMultiMO
	::add(ThingMO& tmo) {
		O3dSchema::thingMOList.add(tmo, firstThingMO_);
		if(tmo.hasAnimation())
			hasAnimation_ = true;
	}


	void ThingMultiMO
	::remove(ThingMO& tmo) {
		O3dSchema::thingMOList.remove(tmo, firstThingMO_);
	}

}
