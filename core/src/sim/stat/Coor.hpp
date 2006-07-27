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
		 * Set the x coordinate from a short value. (x >> COOR_SHIFT) must give the
		 * Area x coordinate of a terrain tile. (x & COOR_INSIDE_TILE_MASK)
		 * must give the x coordinate inside the the terrain tile, ranging from
		 * 0 (left) to (COOR_RES - 1) (right).
		 *
		 * This is the preferred format for the x coordinate on the GameBoy Advance
		 * and inside the platform independent engine. It is also the native
		 * format of the coordinate inside the Coor class.
		 *
		 * @param x The new x value.
		 */
		inline void setX(const coor_t x) {
			x_ = x;
		}

		/**
		 * Set the y coordinate from a short value. One y unit corresponds to
		 * on x or z unit in 3D space.
		 *
		 * @param y The new y value.
		 */
		inline void setY(const coor_t y) {
			y_ = y;
		}

		/**
		 * Set the z coordinate from a short value. (z >> COOR_SHIFT) must give the
		 * Area z coordinate of a terrain tile. (z & COOR_INSIDE_TILE_MASK)
		 * must give the z coordinate inside the the terrain tile, ranging from
		 * 0 (top) to (COOR_RES - 1) (bottom).
		 *
		 * This is the preferred format for the z coordinate on the GameBoy Advance
		 * and inside the platform independent engine. It is also the native
		 * format of the coordinate inside the Coor class.
		 *
		 * @param z The new z value.
		 */
		inline void setZ(const coor_t z) { z_ = z; }

		/**
		 * Set the x and z values of the coordinate from short values. The format
		 * of the values are explained under setXInt and setZInt.
		 *
		 * @param x The new x value.
		 * @param z The new z value.
		 *
		 * @see setX
		 * @see setZ
		 */
		inline void setXZ(const coor_t x, const coor_t z) { x_ = x; z_ = z; }

		/**
		 * Get the x value of the coordinate as a short. See setXInt for details of
		 * the format.
		 *
		 * @returns the x coordinate
		 *
		 * @see setXInt
		 */
		//inline coor_t x() const { return x_; }

		/**
		 * Get the y value of the coordinate as a short. See setYInt for details of
		 * the format.
		 *
		 * @returns the y coordinate
		 *
		 * @see setZInt
		 */
		//inline coor_t y() const { return y_; }

		/**
		 * Get the z value of the coordinate as a short. See setZInt for details of
		 * the format.
		 *
		 * @returns the y coordinate
		 *
		 * @see setYInt
		 */
		//inline coor_t z() const { return z_; }

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

		/**
		 * Get the distance between the coordinates to the second power.
		 * Used for fast collision detecion between objects. The distance
		 * is in the format described int setXInt and setZInt (before
		 * it is squared).
		 *
		 * @returns The squared distance between two coordinates.
		 */
		inline coor_double_t xzDistanceSquared(const Coor& c) const {
			coor_double_t xDist = c.x_ - x_;
			coor_double_t zDist = c.z_ - z_;
			return ((xDist * xDist) + (zDist * zDist));
		}

		/**
		 * Gets the distance between two coordinates along the x or
		 * the z axis, whichever is longest.
		 */
		coor_t xzDistanceLinf(const Coor& c) const;

		/**
		 * Gets the distance between two coordinates along the y axis.
		 */
		coor_t yDistance(const Coor& c) const;

		/**
		 * Calc angle between two coordinates in the xz-plane.
		 *
		 * @return angle in braybrookians (0 - 255).
		 */
		bray_t xzAngleTowards(const Point3& c) const;
	};

}

#endif