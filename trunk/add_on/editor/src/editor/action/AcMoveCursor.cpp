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


#include "AcMoveCursor.hpp"
#include "sim/action/ActionComponent.hpp"
#include "sim/pos/PosComponent.hpp"
#include "sim/physics/PhysicsComponent.hpp"
#include "sim/schema/SimSchema.hpp"
#include "util/error/Log.hpp"

using namespace se_core;

namespace se_editor {
	const AcMoveCursor actionMoveCursor;


	AcMoveCursor
	::AcMoveCursor()
		: se_core::Action("MoveCursor") {
	}


	AcMoveCursor
	::~AcMoveCursor() {
	}


	void AcMoveCursor
	::perform(long when, ActionComponent& perf, Parameter& parameter) const {
		Param* p = static_cast<Param*>(parameter.data(sizeof(Param)));
		
		PosComponent::Ptr pPos(perf);
		PhysicsComponent::Ptr pPhysics(perf);

		static const coor_t tolerance = 0.05f;
		coor_t x = p->cursor_->coor_.x_;
		if(x < -tolerance) {
			x += tolerance;
			x *= -x;
		}
		else if(x > tolerance) {
			x -= tolerance;
			x *= x;
		}
		else {
			x = 0;
		}
		coor_t z = p->cursor_->coor_.z_;
		if(z < -tolerance) {
			z += tolerance;
			z *= -z;
		}
		else if(z > tolerance) {
			z -= tolerance;
			z *= z;
		}
		else {
			z = 0;
		}

		bray_t yaw = BrayT::towards(p->cursor_->face_.yaw_, 0, BRAY_RES >> 1);
		if(*p->isRelative_) {
			p->cursor_->coor_.scale(32);
			pPhysics->nextMove().velocity_.set(p->cursor_->coor_);
			p->cursor_->coor_.reset();
			pPhysics->nextMove().torque_.yaw_ = yaw;
		}
		else {
			Vector3 f(x, 0, z);
			//pPhysics->nextMove().velocity_.set(f);
			pPhysics->nextMove().addForce(f);
			pPhysics->nextMove().angularVelocity_.yaw_ = yaw;
		}
	}



	const AcMoveCursor& AcMoveCursor
	::param(bool& isRelative, ViewPoint& c, Parameter& out) const {
		Param* p = static_cast<Param*>(out.data(sizeof(Param)));
		p->isRelative_ = &isRelative;
		p->cursor_ = &c;
		return *this;
	}
}
