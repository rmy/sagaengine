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


#include "SimplePlayerFactory.hpp"


using namespace se_core;

namespace game {

	SimplePlayerFactory
	::SimplePlayerFactory(String* name)
		: ThingFactory(got_ACTOR, name)
		, radius_(COOR_RES)
		, isPickable_(false)
		, isCollideable_(false)
		, script_(0)
		, physics_(0) {
	}


	SimplePlayerFactory
	::~SimplePlayerFactory() {
	}


	Thing* SimplePlayerFactory
	::create() const {
		Player* a = new Player(name_->get());

		a->setPickable(isPickable_);
		a->setCollideable(isCollideable_);
		a->setDefaultPhysics(physics_);
		a->setCollide(collide_);
		a->nextPos().setRadius(radius_);

		if(script_) {
			a->setDefaultScript(script_);
		}

		return a;
	}


	void SimplePlayerFactory
	::setRadius(float r) {
		radius_ = static_cast<coor_t>(r * COOR_RES);
	}


	void SimplePlayerFactory
	::setPickable(bool isPickable) {
		isPickable_ = isPickable;
	}

	void SimplePlayerFactory
	::setCollideable(bool isCollideable) {
		isCollideable_ = isCollideable;
	}


	void SimplePlayerFactory
	::setScript(const char* name) {
		script_ = SimSchema::sortedSimObjectList().script(name);
	}


	void SimplePlayerFactory
	::setPhysics(const char* name) {
		physics_ = SimSchema::sortedSimObjectList().physics(name);
	}


	void SimplePlayerFactory
	::setCollide(const char* name) {
		collide_ = SimSchema::sortedSimObjectList().collide(name);
	}


}
