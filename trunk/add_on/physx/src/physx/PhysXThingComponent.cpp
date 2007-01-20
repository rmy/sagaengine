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


#include "PhysXThingComponent.hpp"
#include "PhysXManager.hpp"

using namespace se_core;

namespace se_physx {

	PhysXThingComponent
	::PhysXThingComponent(SimComposite* owner)
		: SimNodeComponent(sct_ODE, owner) {
	}


	PhysXThingComponent
	::~PhysXThingComponent() {
	}


	void PhysXThingComponent
	::setActive(bool state) {
		if(state) {
			PosComponent* pPos = PosComponent::get(*this);
			const BoundingBox& b = pPos->pos().bounds_;

			NxActorDesc actorDesc;
			actorDesc.setToDefault();
			NxBodyDesc bodyDesc;
			NxBoxShapeDesc boxDesc;

			boxDesc.dimensions.min(NxVec3(b.minX_, b.minY_, b.minZ_));
			boxDesc.dimensions.max(NxVec3(b.maxX_, b.maxY_, b.maxZ_));
			actorDesc.shapes.push_back(&boxDesc);

			actorDesc.body = &bodyDesc;
			actorDesc.density = 10;
			const Point3& p = pPos->pos().worldCoor();
			actorDesc.globalPose.t = NxVec3(p.x_, p.y_ + 5, p.z_);
			Assert(actorDesc.isValid());

			actor_ = PhysXManager::singleton().createActor(actorDesc);
			Assert(!actor_->isSleeping())

		}
		else {
			PhysXManager::singleton().destroyActor(*actor_);
		}
	}


	void PhysXThingComponent
	::update() {
		PosComponent* pPos = PosComponent::get(*this);
		NxVec3 &v = actor_->getGlobalPosition();
		pPos->nextPos().worldCoor().set(v.x, v.y, v.z);

		//NxQuat &q = actor_->getGlobalOrientationQuat();
		//pPos->nextPos().worldFace().set(Quat4(q.x, q.y, q.z, q.w));
		//pPos->nextPos().worldFace().yaw_ = BrayT::invert(pPos->nextPos().worldFace().yaw_);
		pPos->nextPos().updateLocalViewPoint();

		//if(!pPos->nextPos().worldCoor().equals(pPos->pos().worldCoor())) {
		//	LogWarning("Did move! " << owner_->name() << pPos->nextPos().worldCoor() << pPos->pos().worldCoor());
		//}
	}

	void PhysXThingComponent
	::applyForces() {
		PhysicsComponent* pPhysics = PhysicsComponent::get(*this);

		const Vector3& f = pPhysics->nextMove().force_;
		NxVec3 nxF(f.x_, f.y_, f.z_);
		actor_->addForce(nxF);
	}

}
