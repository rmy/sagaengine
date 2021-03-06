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


#include "AcGrabNearest.hpp"
#include "sim/schema/SimSchema.hpp"
#include "util/error/Log.hpp"
#include "sim/action/ActionAndParameter.hpp"
#include "sim/action/ActionComponent.hpp"
#include "sim/pos/PosComponent.hpp"
#include "sim/physics/PhysicsComponent.hpp"
#include "editor/comp/EditorAreaComponent.hpp"
#include "editor/comp/EditorComponent.hpp"
#include "editor/schema/EditorSchema.hpp"

using namespace se_core;

namespace se_editor {
	const AcGrabNearest actionGrabNearest;

	AcGrabNearest
	::AcGrabNearest() : se_core::Action("GrabNearest") {
	}


	AcGrabNearest
	::~AcGrabNearest() {
	}


	void AcGrabNearest
	::perform(long when, ActionComponent& perf, Parameter& parameter) const {
		PosComponent::Ptr pPos(perf);
		PhysicsComponent::Ptr pPhysics(perf);
		PosComponent::Ptr aPos(*pPos->pos().area());
		EditorAreaComponent::Ptr aEditor(*pPos->pos().area());
		EditorComponent* c = aEditor->findNearest(pPos->nextPos().local_.coor_);
		if(c) {
			PosComponent::Ptr cPos(*c);
			pPos->nextPos().world_.setViewPoint(cPos->pos().world_);
			pPos->nextPos().updateLocalViewPoint();
			pPhysics->nextMove().work_.vp_.face_.yaw_ = pPos->nextPos().worldFace().yaw_;

			cPos->nextPos().setParent(*pPos);
			cPos->nextPos().local_.setIdentity();
			cPos->nextPos().updateWorldViewPoint();

			EditorSchema::lastSpawn = c->owner();
		}
	}

}
