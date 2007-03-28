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


#include "AcExitEditor.hpp"
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

using namespace se_core;
using namespace se_client;

namespace se_editor {
	const AcExitEditor actionExitEditor;

	AcExitEditor
	::AcExitEditor()
		: se_core::Action("ExitEditor") {
	}


	AcExitEditor
	::~AcExitEditor() {
	}


	bool AcExitEditor
	::isContinuing(se_core::ActionComponent &performer, se_core::Parameter& parameter) const {
		return parameter.actionStage() < 2;
	}

	void AcExitEditor
	::perform(long when, ActionComponent& perf, Parameter& parameter) const {

		switch(parameter.actionStage()) {
		case 0: 
			{
				EditorControls::singleton().loseFocus();
				ScriptComponent::Ptr pScript(perf);
				pScript->popScript();

				PhysicsComponent::Ptr cPhysics(*ClientSchema::camera);
				cPhysics->popPhysics();

				EditorManager::singleton().exitEditor();
			}
			break;

		case 1:
			{
				PosComponent::Ptr pPos(perf);
				pPos->nextPos().bounds_.minX_ *= 2;
				pPos->nextPos().bounds_.maxX_ *= 2;
				pPos->nextPos().bounds_.minZ_ *= 2;
				pPos->nextPos().bounds_.maxZ_ *= 2;
				CollisionManager::singleton().resetGodMode();
			}
			break;
		}
	}

}
