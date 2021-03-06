/*
   Copyright (C) 1997,1998,1999
   Kenji Hiranabe, Eiwa System Management, Inc.

   This program is free software.
   Implemented by Kenji Hiranabe(hiranabe@esm.co.jp),
   conforming to the Java(TM) 3D API specification by Sun Microsystems.

   Permission to use, copy, modify, distribute and sell this software
   and its documentation for any purpose is hereby granted without fee,
   provided that the above copyright notice appear in all copies and
   that both that copyright notice and this permission notice appear
   in supporting documentation. Kenji Hiranabe and Eiwa System Management,Inc.
   makes no representations about the suitability of this software for any
   purpose.  It is provided "AS IS" with NO WARRANTY.

   Copyright (C) 2004
   Rune Myrland, Skalden Studio AS
   SagaEngine adaptions, involving conversion from floating point values to fixed point values.
   Kenji Hiranabe's license apply.
*/
#ifndef base_vecmath_Point3_hpp
#define base_vecmath_Point3_hpp

#include "util_vecmath.hpp"
#include "Vector3.hpp"

namespace se_core {
	/**
	 * A 3 element point that is represented by x,y,z coordinates.
	 * @author Kenji Hiranabe, Rune Myrland
	 */
	class _SeCoreExport Point3 : public Vector3 {
	public:
		/**
		 * Constructs and initializes a Point3 from the specified xyz coordinates.
		 * @param x the x coordinate
		 * @param y the y coordinate
		 * @param z the z coordinate
		 */
		Point3(coor_t x, coor_t y, coor_t z): Vector3(x, y, z) { }

		/**
		 * Constructs and initializes a Point3 from the specified array.
		 * @param p the array of length 3 containing xyz in order
		 */
		Point3(const coor_t p[]): Vector3(p) { }

		/*
		 * Constructs and initializes a Point3 from the specified Tuple3d.
		 * @param t1 the Tuple3d containing the initialization x y z data
		 */
		Point3(const Tuple3& t1): Vector3(t1) { }

		/**
		 * Constructs and initializes a Point3 to (0,0,0).
		 */
		Point3(): Vector3() { }

		/**
		 * Computes the square of the distance between this point and point p1.
		 * @param  p1 the other point
		 * @return the square of distance between these two points as a float
		 */
		coor_double_t distanceSquared(const Point3& p1) const {
			coor_double_t dx = x_ - p1.x_;
			coor_double_t dy = y_ - p1.y_;
			coor_double_t dz = z_ - p1.z_;
			return dx*dx + dy*dy + dz*dz;
		}

		/**
		 * Returns the distance between this point and point p1.
		 * @param p1 the other point
		 * @return the distance between these two points as a float
		 */
		coor_t distance(const Point3& p1) const;

		/**
		 * Computes the L-1 (Manhattan) distance between this point and point p1.
		 * The L-1 distance is equal to abs(x1-x2) + abs(y1-y2).
		 * @param p1 the other point
		 */
		coor_t distanceL1(const Point3& p1) const;

		/**
		 * Computes the L-infinite distance between this point and point p1.
		 * The L-infinite distance is equal to MAX[abs(x1-x2), abs(y1-y2)].
		 * @param p1 the other point
		 */
		coor_t distanceLinf(const Point3& p1) const;

		/**
		 * Multiplies each of the x,y,z components of the Point4 parameter
		 * by 1/w and places the projected values into this point.
		 * @param p1 the source Point4, which is not modified
		 */
		void project(const Point4& p1);


		/**
		 * Get the distance between the coordinates to the second power.
		 * Used for fast collision detecion between objects. The distance
		 * is in the format described int setXInt and setZInt (before
		 * it is squared).
		 *
		 * @returns The squared distance between two coordinates.
		 */
		inline coor_double_t xzDistanceSquared(const Point3& c) const {
			coor_double_t xDist = c.x_ - x_;
			coor_double_t zDist = c.z_ - z_;
			return ((xDist * xDist) + (zDist * zDist));
		}

		coor_t xzDistance(const Point3& p1) const;

		/**
		 * Gets the distance between two coordinates along the x or
		 * the z axis, whichever is longest.
		 */
		coor_t xzDistanceLinf(const Point3& c) const;

		/**
		 * Gets the distance between two coordinates along the y axis.
		 */
		coor_t yDistance(const Point3& c) const;

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
		 * Calc angle between two coordinates in the xz-plane.
		 *
		 * @return angle in braybrookians (0 - 255).
		 */
		bray_t yawTowards(const Point3& c) const;

		void eulerTowards(const Point3& c, Euler3& dest) const;

		void nearestPoint(const Point3& pt1, const Point3& pt2, const Point3& testPoint);

		// copy constructor and operator = is made by complier

		Point3& operator=(const Tuple3& t) {
			Tuple3::operator=(t);
			return *this;
		}
	};

} // Namespace

/*
inline
se_core::Point3 operator*(scale_t s, const se_core::Point3& t1) {
    return operator*(s, (const se_core::Tuple3&)t1);
}
*/


#endif /* POINT3_H */
