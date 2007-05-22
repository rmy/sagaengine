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


#ifndef logic_physics_PhSimple_hpp
#define logic_physics_PhSimple_hpp

#include "LogicPre.hpp"
#include "sim/physics/Physics.hpp"
#include "sim/physics/Move.hpp"
#include "sim/thing/Actor.hpp"

namespace logic {

	class _FlyLogicExport PhSimple : public se_core::Physics {
	public:
		PhSimple() : se_core::Physics("Simple") {}

		void calcNext(const se_core::PhysicsComponent& actor
				, const se_core::Pos& pos
				, se_core::Pos& nextPos
				, const se_core::Move& move
				, se_core::Move& nextMove
				) const;

		bool isBlocked(const se_core::PhysicsComponent& actor
				, const se_core::Pos& pos
				, const se_core::Pos& nextPos
				) const;

		void affect(se_core::PhysicsComponent& actor) const {}

	private:
	};
}

#endif
