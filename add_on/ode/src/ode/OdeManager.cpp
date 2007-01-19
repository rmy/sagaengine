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


#include "OdePre.hpp"
#include "OdeManager.hpp"
#include "OdeAreaComponent.hpp"

using namespace se_core;

namespace se_ode {

	OdeManager
	::OdeManager() 
		: SimComponentManager(sct_ODE), contactCount_(0), activeContactCount_(0) {
		worldId_ = dWorldCreate();
		spaceId_ = dHashSpaceCreate(0);
		collisionJointGroup_ = dJointGroupCreate (0);
	}


	OdeManager
	::~OdeManager() {
		dJointGroupDestroy(collisionJointGroup_);
		dSpaceDestroy(spaceId_);
		dWorldDestroy(worldId_);
		dCloseODE();
	}


	OdeManager&  OdeManager
	::singleton() {
		static OdeManager instance;
		return instance;
	}


	void collisionCallback(void *data, dGeomID o1, dGeomID o2) {
		static_cast<OdeManager*>(data)->collision(o1, o2);
	}


	void OdeManager
	::collision(dGeomID o1, dGeomID o2) {
		contactCount_ += dCollide(o1, o2, MAX_CONTACTS - contactCount_, &contacts_[contactCount_].geom, sizeof(contacts_[0]));
	}


	void OdeManager
	::step(long when) {
		// Flip coors for next physics step
		//flip(when);

		// Precalc coordinate for the next frame
		//performPhysics(when + TIMESTEP_INTERVAL);

		//Apply forces to the bodies as necessary. 
		applyForces();

		// Adjust joints parameters as necessary. 
		//Call collision detection. 
		dSpaceCollide(spaceId_, collisionJointGroup_, collisionCallback);

		// Create a contact joint for every collision point, and put it in the contact joint group
		for(int i = 0; i < contactCount_; ++i) {
			contacts_[i].surface.mode = dContactBounce;
			contacts_[i].surface.bounce = 1;
			dJointID joint = dJointCreateContact(worldId_, collisionJointGroup_, &contacts_[i]);
		}

		// Take a simulation step. 
		dWorldStep(worldId_, SCALE_STEP);

		// Remove all joints in the contact joint group.
		dJointGroupEmpty(collisionJointGroup_);
		contactCount_ = 0;
		activeContactCount_ = 0;

		update();
		// Test and resolve collisions between game things and between
		// game things and walls or other terrain obstructions.
		//testCollisions(when, when + TIMESTEP_INTERVAL);

		// Affect thing with effects of movements
		//affect(when + TIMESTEP_INTERVAL);
	}


	void OdeManager
	::applyForces() {
		MultiSimNodeComponent::Iterator it(children_);
		while(it.hasNext()) {
			OdeAreaComponent* oac = static_cast<OdeAreaComponent*>(&it.next());
			oac->applyForces();
		}
	}


	void OdeManager
	::update() {
		MultiSimNodeComponent::Iterator it(children_);
		while(it.hasNext()) {
			OdeAreaComponent* oac = static_cast<OdeAreaComponent*>(&it.next());
			oac->update();
		}
	}


	dBodyID OdeManager
	::createBody() {
		return dBodyCreate(worldId_);
	}


	void OdeManager
	::destroyBody(dBodyID id) {
		dBodyDestroy(id);
	}


	dSpaceID OdeManager
	::createSpace() {
		return dHashSpaceCreate(spaceId_);
	}


	void OdeManager
	::destroySpace(dSpaceID id) {
		dSpaceDestroy(id);
	}
}
