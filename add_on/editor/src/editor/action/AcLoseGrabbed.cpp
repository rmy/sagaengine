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


#include "AcLoseGrabbed.hpp"
#include "sim/schema/SimSchema.hpp"
#include "util/error/Log.hpp"
#include "sim/action/ActionAndParameter.hpp"
#include "sim/action/ActionComponent.hpp"
#include "sim/pos/PosComponent.hpp"
#include "editor/comp/EditorAreaComponent.hpp"
#include "editor/comp/EditorComponent.hpp"
#include "editor/schema/EditorSchema.hpp"

using namespace se_core;

namespace se_editor {
	const AcLoseGrabbed actionLoseGrabbed;

	AcLoseGrabbed
	::AcLoseGrabbed() : se_core::Action("LoseGrabbed") {
	}


	AcLoseGrabbed
	::~AcLoseGrabbed() {
	}


	void AcLoseGrabbed
	::perform(long when, ActionComponent& perf, Parameter& parameter) const {
		if(EditorSchema::lastSpawn) {
			PosComponent::Ptr pPos(perf);


			PosComponent::Ptr lsPos(*EditorSchema::lastSpawn);
			lsPos->nextPos().resetParent(true);
			EditorComponent::Ptr lsEditor(*EditorSchema::lastSpawn);

			PosComponent* area = lsEditor->startArea();
			if(area) {
				lsPos->nextPos().setArea(*lsEditor->startArea());
				lsPos->nextPos().local_.setViewPoint(lsEditor->start());
				lsPos->nextPos().updateWorldViewPoint();
			}
			else {
				lsPos->owner()->scheduleForDestruction();
			}

			EditorSchema::lastSpawn = 0;
			return;
		}
	}

}
