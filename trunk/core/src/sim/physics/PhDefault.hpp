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


#ifndef engine_physics_PhDefault_hpp
#define engine_physics_PhDefault_hpp

#include "Physics.hpp"
#include "util/vecmath/Vector3.hpp"

namespace se_core {

	/** A default movement formula. */
	class _SeCoreExport PhDefault : public Physics {
	public:
		PhDefault() : Physics("Default") {}
		void calcNext(const se_core::PhysicsComponent& physics
				, const se_core::Pos& pos
				, se_core::Pos& nextPos
				, const se_core::Move& move
				, se_core::Move& nextMove
				) const;

		bool isBlocked(const se_core::PhysicsComponent& physics
				, const se_core::Pos& pos
				, se_core::Pos& nextPos
				) const;

		virtual void blocked(const se_core::PhysicsComponent& physics
				, const se_core::Pos& pos
				, se_core::Pos& nextPos
				, const se_core::Move& move
				, se_core::Move& nextMove
				) const;
		void affect(se_core::PhysicsComponent& physics) const {}

	protected:
		void blendAnims(const se_core::Pos& pos, se_core::Pos& nextPos) const;
		void revertXZ(const se_core::Pos& pos, se_core::Pos& nextPos) const;
		void updateForces(se_core::Move& nextMove) const;
		void applyFriction(se_core::Move& nextMove) const;
		void applyForces(const se_core::Pos& pos, se_core::Pos& nextPos, const se_core::Move& move, se_core::Move& nextMove) const;
		void clampToGround(const se_core::Pos& pos, se_core::Pos& nextPos, se_core::Move& nextMove) const;

		static const se_core::Vector3 GRAVITY;
	};


	extern _SeCoreExport const PhDefault physicsDefault;


}

#endif
