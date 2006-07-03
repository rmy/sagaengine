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


#ifndef game_area_SimpleArea_hpp
#define game_area_SimpleArea_hpp

#include "GamePre.H"

namespace game {

	class SimpleArea : public se_core::Area {
	public:
		enum TerrainStyle {
			TS_VOID = se_core::Pos::TS_VOID,
			TS_WALKABLE
		};


		SimpleArea(se_core::String* name, coor_tile_t w, coor_tile_t h);
		virtual ~SimpleArea();

		void updateIndex(se_core::Pos&) const {}
		coor_t groundHeight(const se_core::Coor& coor, short index = 0) const;
		void force(const se_core::Coor& coor, se_core::Force& dest) const;

		short terrainStyle(const se_core::Coor& coor, short index = 0) const;
		short nextTerrainStyle(bray_t direction, const se_core::Coor& coor);

	private:
	};

}

#endif
