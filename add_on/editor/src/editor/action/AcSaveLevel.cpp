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


#include "AcSaveLevel.hpp"
#include "sim/action/ActionComponent.hpp"
#include "sim/pos/PosComponent.hpp"
#include "sim/script/ScriptComponent.hpp"
#include "sim/physics/PhysicsComponent.hpp"
#include "sim/physics/PhysicsManager.hpp"
#include "sim/react/CollisionManager.hpp"
#include "sim/schema/SimSchema.hpp"
#include "sim/area/AreaManager.hpp"
#include "util/error/Log.hpp"
#include "client/schema/ClientSchema.hpp"
#include "../input/EditorControls.hpp"
#include "../physics/PhCameraAbove.hpp"
#include "../comp/EditorManager.hpp"
#include "../comp/EditorAreaComponent.hpp"

using namespace se_core;
using namespace se_client;

namespace se_editor {
	const AcSaveLevel actionSaveLevel;

	AcSaveLevel
	::AcSaveLevel()
		: se_core::Action("SaveLevel") {
	}


	AcSaveLevel
	::~AcSaveLevel() {
	}


	bool AcSaveLevel
	::isContinuing(se_core::ActionComponent &performer, se_core::Parameter& parameter) const {
		return false;
	}

	void AcSaveLevel
	::perform(long when, ActionComponent& perf, Parameter& parameter) const {
		PosComponent::Ptr pPos(perf);
		EditorAreaComponent::Ptr aEdit(pPos->nextPos().area());
		aEdit->save();

		ScriptComponent::Ptr pScript(perf);
		pScript->popScript();

		PhysicsComponent::Ptr pPhysics(perf);
		pPhysics->popPhysics();

		PhysicsComponent::Ptr cPhysics(*ClientSchema::camera);
		cPhysics->popPhysics();

		EditorControls::singleton().loseFocus();
		EditorManager::singleton().exitEditor();

		pPos->nextPos().bounds_.minX_ *= 3;
		pPos->nextPos().bounds_.maxX_ *= 3;
		pPos->nextPos().bounds_.minZ_ *= 3;
		pPos->nextPos().bounds_.maxZ_ *= 3;
		CollisionManager::singleton().resetGodMode();
	}

}
