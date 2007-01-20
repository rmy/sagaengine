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


#include "PhysXPre.hpp"
#include "PhysXManager.hpp"
#include "PhysXAreaComponent.hpp"

using namespace se_core;

namespace se_physx {

	PhysXManager
	::PhysXManager() 
		: SimComponentManager(sct_ODE) {
		sdk_ = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION);
		AssertFatal(sdk_, "Couldn't create PhysX SDK");

		sdk_->getFoundationSDK().getRemoteDebugger()->connect ("localhost", 5425);

		// Set the physics parameters
		sdk_->setParameter(NX_SKIN_WIDTH, 0.01f);

		// Set the debug visualization parameters
		sdk_->setParameter(NX_VISUALIZATION_SCALE, 1);
		sdk_->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
		sdk_->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);

		// Create the scene
		NxSceneDesc sceneDesc;
		sceneDesc.simType = NX_SIMULATION_HW;
		const NxVec3 GRAVITY(0, -9.8f, 0);
		sceneDesc.gravity = GRAVITY;
		scene_ = sdk_->createScene(sceneDesc);	
		if(!scene_){
			sceneDesc.simType = NX_SIMULATION_SW; 
			scene_ = sdk_->createScene(sceneDesc);  
		}

		AssertFatal(scene_, "Couldn't create PhysX scene");

		// Create the default material
		NxMaterial* defaultMaterial = scene_->getMaterialFromIndex(0);
		defaultMaterial->setRestitution(0.5);
		defaultMaterial->setStaticFriction(0.5);
		defaultMaterial->setDynamicFriction(0.5);
	}


	PhysXManager
	::~PhysXManager() {
		sdk_->release();
	}


	PhysXManager&  PhysXManager
	::singleton() {
		static PhysXManager instance;
		return instance;
	}

	/*

	void PhysXManager
	::collision(dGeomID o1, dGeomID o2) {
		contactCount_ += dCollide(o1, o2, MAX_CONTACTS - contactCount_, &contacts_[contactCount_].geom, sizeof(contacts_[0]));
	}
	*/


	void PhysXManager
	::step(long when) {
		// Flip coors for next physics step
		//flip(when);

		// Precalc coordinate for the next frame
		//performPhysics(when + TIMESTEP_INTERVAL);

		//Apply forces to the bodies as necessary. 
		applyForces();

		// Adjust joints parameters as necessary. 
		//Call collision detection. 

		// Create a contact joint for every collision point, and put it in the contact joint group

		// Take a simulation step. 
		scene_->simulate(SCALE_STEP);
		scene_->flushStream();
		//dWorldStep(worldId_, SCALE_STEP);

		// Remove all joints in the contact joint group.

		update();
		// Test and resolve collisions between game things and between
		// game things and walls or other terrain obstructions.
		//testCollisions(when, when + TIMESTEP_INTERVAL);

		// Affect thing with effects of movements
		//affect(when + TIMESTEP_INTERVAL);
	}


	void PhysXManager
	::applyForces() {
		MultiSimNodeComponent::Iterator it(children_);
		while(it.hasNext()) {
			PhysXAreaComponent* oac = static_cast<PhysXAreaComponent*>(&it.next());
			oac->applyForces();
		}
	}


	void PhysXManager
	::update() {
		while (!scene_->fetchResults(NX_RIGID_BODY_FINISHED, false));
		MultiSimNodeComponent::Iterator it(children_);
		while(it.hasNext()) {
			PhysXAreaComponent* oac = static_cast<PhysXAreaComponent*>(&it.next());
			oac->update();
		}
	}


	NxActor* PhysXManager
	::createActor(NxActorDesc& desc) {
		return scene_->createActor(desc);
	}


	void PhysXManager
	::destroyActor(NxActor& actor) {
		scene_->releaseActor(actor);
	}

	/*

	dSpaceID PhysXManager
	::createSpace() {
		return dHashSpaceCreate(spaceId_);
	}


	void PhysXManager
	::destroySpace(dSpaceID id) {
		dSpaceDestroy(id);
	}
	*/
}
