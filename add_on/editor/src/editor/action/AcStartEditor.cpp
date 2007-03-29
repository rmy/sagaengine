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


#include "AcStartEditor.hpp"
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
	const AcStartEditor actionStartEditor;

	AcStartEditor
	::AcStartEditor()
		: se_core::Action("StartEditor") {
	}


	AcStartEditor
	::~AcStartEditor() {
	}


	void AcStartEditor
	::perform(long when, ActionComponent& perf, Parameter& parameter) const {
		PosComponent::Ptr pPos(perf);
		pPos->nextPos().bounds_.minX_ *= 1 / 3.0f;
		pPos->nextPos().bounds_.maxX_ *= 1 / 3.0f;
		pPos->nextPos().bounds_.minZ_ *= 1 / 3.0f;
		pPos->nextPos().bounds_.maxZ_ *= 1 / 3.0f;

		ScriptComponent::Ptr pScript(perf);

		EditorControls::singleton().grabFocus();
		pScript->pushScript( &EditorControls::singleton() );

		PhysicsComponent::Ptr cPhysics(*ClientSchema::camera);
		cPhysics->pushPhysics( &physicsCameraAbove );

		CollisionManager::singleton().resetGodMode();
		CollisionManager::singleton().flipGodMode();

		EditorManager::singleton().startEditor();
	}

}
