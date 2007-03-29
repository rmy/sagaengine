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


#include "AcPlaceEntrance.hpp"
#include "sim/action/ActionComponent.hpp"
#include "sim/pos/PosComponent.hpp"
#include "editor/comp/EditorAreaComponent.hpp"
#include "sim/physics/PhysicsComponent.hpp"
#include "sim/schema/SimSchema.hpp"
#include "util/error/Log.hpp"

using namespace se_core;

namespace se_editor {
	const AcPlaceEntrance actionPlaceEntrance;


	AcPlaceEntrance
	::AcPlaceEntrance()
		: se_core::Action("PlaceEntrance") {
	}


	AcPlaceEntrance
	::~AcPlaceEntrance() {
	}


	void AcPlaceEntrance
	::perform(long when, ActionComponent& perf, Parameter& parameter) const {
		Param* p = static_cast<Param*>(parameter.data(sizeof(Param)));

		PosComponent::Ptr pPos(perf);
		EditorAreaComponent::Ptr aEditor(*pPos->pos().area());
		ViewPoint vp;
		pPos->pos().areaViewPoint(*pPos->pos().area(), vp);

		aEditor->setEntrance(p->id_, vp);
	}



	const AcPlaceEntrance& AcPlaceEntrance
	::param(int id, Parameter& out) const {
		Param* p = static_cast<Param*>(out.data(sizeof(Param)));
		p->id_ = id;
		return *this;
	}
}
