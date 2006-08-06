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


#ifndef sim_stat_Coor_hpp
#define sim_stat_Coor_hpp

#include "sim_stat.hpp"
#include "SpawnPoint.hpp"
#include "util/math/CoorT.hpp"
#include "util/vecmath/Point3.hpp"
#include "util/vecmath/Quat4.hpp"
#include "../area/sim_area.hpp"

namespace se_core {

	class Coor : public Point3 {
	public:
		/**
		 * Constructor setting default values for the coordinate.
		 */
		Coor() : Point3() {
		}

		/**
		 * Constructor setting default values for the coordinate.
		 *
		 * @param x the x coordinate
		 * @param y the y coordinate
		 * @param z the z coordinate
		 */
		Coor(coor_t x, coor_t y, coor_t z)
				: Point3(x, y, z) {
		}

		/**
		 * Get the area terrain tile x coordinate. Use this
		 * to get the terrain tile from Area.
		 *
		 * @return the area terrain tile x coordinate
		 */
		inline coor_tile_t xTile() const { return CoorT::tile(x_); }

		/**
		 * Get the area terrain tile z coordinate. Use this
		 * to get the terrain tile from Area.
		 *
		 * @return the area terrain tile z coordinate
		 */
		inline coor_tile_t zTile() const { return CoorT::tile(z_); }

		/**
		 * Get the x coordinate inside the terrain tile. Use this
		 * when you have already gotten the correct terrain tile
		 * from Area, and wants to know where you
		 * are inside the tile. The value is between 0 (left)
		 * and (COOR_RES - 1) (right).
		 *
		 * @returns z coordinate inside the terrain tile.
		 */
		coor_in_t xInsideTile() const { return CoorT::inTile(x_); }

		/**
		 * Get the z coordinate inside the terrain tile. Use this
		 * when you have already gotten the correct terrain tile
		 * from Area, and wants to know where you
		 * are inside the tile. The value is between 0 (top)
		 * and (COOR_RES - 1) (bottom).
		 *
		 * @returns z coordinate inside the terrain tile.
		 */
		coor_in_t zInsideTile() const { return CoorT::inTile(z_); }

	};

}

#endif
