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


#ifndef game_physics_PhNavMesh_hpp
#define game_physics_PhNavMesh_hpp

#include "GamePre.H"

namespace game {

	class PhNavMesh : public se_core::Physics {
	public:
		PhNavMesh() : se_core::Physics("NavMesh") {}

		void calcNext(const se_core::Actor& actor
				, const se_core::Pos& pos
				, se_core::Pos& nextPos
				, se_core::Anim& nextAnim
				, se_core::Move& nextMove
				) const;

		bool isBlocked(const se_core::Actor& actor
				, const se_core::Pos& pos
				, const se_core::Pos& nextPos
				) const;

		void affect(se_core::Actor& actor) const {}

	private:
	};
}

#endif
