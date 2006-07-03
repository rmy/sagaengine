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


#ifndef game_area_NavMeshArea_hpp
#define game_area_NavMeshArea_hpp

#include "game_area.hpp"
#include "GamePre.H"

namespace game {

	class NavMeshArea : public se_core::Area {
	public:
		enum TerrainStyle {
			TS_VOID = se_core::Pos::TS_VOID,
			TS_WALKABLE
		};


		NavMeshArea(se_core::String* name, coor_tile_t w, coor_tile_t h, se_core::ByteArray* data);
		virtual ~NavMeshArea();

		void updateIndex(se_core::Pos& pos) const;
		coor_t groundHeight(const se_core::Coor& coor, short index = -1) const;

		void force(const se_core::Coor& coor, se_core::Force& dest) const;

		short terrainStyle(const se_core::Coor& coor, short index = -1) const;
		short nextTerrainStyle(bray_t direction, const se_core::Coor& coor);

		bool isLineOfSight(const se_core::Pos& from, const se_core::Pos& to);
		void path(const se_core::Pos& from, const se_core::Pos& to, se_core::Point3& out) const;

	private:
		se_core::ByteArray* data_;
		const NavMesh* navMesh_;
	};

}

#endif
