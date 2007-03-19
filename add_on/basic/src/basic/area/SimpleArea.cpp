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


#include "BasicPre.hpp"
#include "SimpleArea.hpp"

using namespace se_core;

namespace se_basic {
	SimpleArea
	::SimpleArea(const CompositeFactory* f, String* name, coor_tile_t w, coor_tile_t h)
		: Area (f, name, w, h) {
	}


	SimpleArea
	::~SimpleArea() {
	}


	coor_t SimpleArea
	::groundHeight(const Point3& coor, short index) const {
		// Simple areas area flat
		return 0;
	}


	void SimpleArea
	::force(const Point3& coor, Vector3& dest) const {
		// No up or down in SimpleArea
		dest.set(0, 0, 0);
	}


	short SimpleArea
	::terrainStyle(const Point3& coor, short index) const {
		// Only one rule - don't walk outside the area.

		coor_tile_t x = coor.xTile();
		coor_tile_t y = coor.zTile();

		if(!isLegalCoor(x, y)) {
			return TS_VOID;
		}

		return TS_WALKABLE;
	}

	short SimpleArea
	::nextTerrainStyle(bray_t direction, const Point3& coor) {
		// May be deprecated soon??
		LogFatal("Not implemented");
		return 0;
	}

}
