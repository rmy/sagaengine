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


#include "OdeThingComponent.hpp"
#include "OdeManager.hpp"

using namespace se_core;

namespace se_ode {

	OdeThingComponent
	::OdeThingComponent(SimComposite* owner)
		: SimNodeComponent(sct_ODE, owner) {
	}


	OdeThingComponent
	::~OdeThingComponent() {
	}


	void OdeThingComponent
	::setActive(bool state) {
		if(state) {
			bodyId_ = OdeManager::singleton().createBody();
			PosComponent* pPos = PosComponent::get(*this);
			const Point3& p = pPos->pos().worldCoor();
			dBodySetPosition(bodyId_, p.x_, p.y_ + 3, p.z_);
			Quat4 q(pPos->pos().worldFace());
			dReal q2[4];
			q2[0] = q.w_;
			q2[0] = q.x_;
			q2[0] = q.y_;
			q2[0] = q.z_;
			dBodySetQuaternion(bodyId_, q2);

			const BoundingBox& b = pPos->pos().bounds_;
			geomId_ = dCreateCCylinder(0, b.radius(), b.maxY_ - b.minY_);
			dGeomSetBody(geomId_, bodyId_);
		}
		else {
			dGeomDestroy(geomId_);
			OdeManager::singleton().destroyBody(bodyId_);
		}
	}


	void OdeThingComponent
	::update() {
		PosComponent* pPos = PosComponent::get(*this);
		const dReal* p = dBodyGetPosition(bodyId_);
		pPos->nextPos().worldCoor().set(p);
		const dReal* q = dBodyGetQuaternion(bodyId_);
		pPos->nextPos().worldFace().set(Quat4(q[1], q[2], q[3], q[0]));
	}

	void OdeThingComponent
	::applyForces() {
		PhysicsComponent* pPhysics = PhysicsComponent::get(*this);

		const Vector3& f = pPhysics->nextMove().force_;
		dBodyAddForce(bodyId_, f.x_, f.y_, f.z_);
	}

}
