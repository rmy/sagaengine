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


#include "PhSpawnPos.hpp"
#include "sim/schema/SimSchema.hpp"
#include "util/error/Log.hpp"
#include "sim/pos/all.hpp"
#include "sim/physics/all.hpp"
#include "../comp/EditorComponent.hpp"

using namespace se_core;

namespace se_editor {
	const PhSpawnPos physicsSpawnPos;


	void PhSpawnPos
	::calcNext(const PhysicsComponent& physics
			, const Pos& pos
			, Pos& nextPos
			, const Move& move
			, Move& nextMove
			) const {
	
		// Are physics meaningful at all?
		if(!pos.isKeyFramePath(nextPos)) {
			// If not skip it
			return;
		}

		EditorComponent::Ptr pEditor(physics);
		// 

		nextPos.updateLocalViewPoint();
	}


	void PhSpawnPos
	::affect(PhysicsComponent& physics) const {
		Pos& nextPos = PosComponent::Ptr(physics)->nextPos();
		if(!nextPos.hasArea()) {
			// Camera has left all areas - which means
			// it should be destroyed
			physics.owner()->scheduleForDestruction();
		}
	}
}
