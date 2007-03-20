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


#ifndef se_editor_SpawnPos_hpp
#define se_editor_SpawnPos_hpp

#include "sim/sim.hpp"
#include "sim/physics/Physics.hpp"

namespace se_editor {
	class _SeEditorExport PhSpawnPos : public se_core::Physics {
	public:
		PhSpawnPos() : se_core::Physics("SpawnPos") {}

		void calcNext(const se_core::PhysicsComponent& physics
				, const se_core::Pos& pos
				, se_core::Pos& nextPos
				, const se_core::Move& move
				, se_core::Move& nextMove
				) const;

		void affect(se_core::PhysicsComponent& physics) const;

	private:
	};


	extern _SeEditorExport const PhSpawnPos physicsSpawnPos;

}

#endif
