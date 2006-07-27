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


#ifndef Pos_hpp
#define Pos_hpp

#include "ViewPoint.hpp"
#include "../sim.hpp"
#include "../area/sim_area.hpp"
#include "../config/sim_config.hpp"


namespace se_core {
	/**
	 * Position, movement and volume info.
	 * The Pos class maintains coordinates, volume and movement info for Thing
	 * and subclasses.
	 */
	class Pos : public ViewPoint {
	public:
		static const short TS_VOID = 0;
		static const long TSM_VOID = (1 << TS_VOID);

		/**
		 * A mapping of some common directions to Braybrookians in the xz-plane.
		 */
		enum Direction {
			EAST = 0, SOUTH_EAST = 32,
			SOUTH = 64, SOUTH_WEST = 96,
			WEST = 128, NORTH_WEST = 160,
			NORTH = 192, NORTH_EAST = 224
		};

		/**
		 * Constructor.
		 */
		Pos();

		/**
		 * Copy all values of original into this.
		 */
		void setPos(const Pos& original);


		inline void setCoor(const Coor& original) {
			coor_.set(original);
		}



		/**
		 * Area.
		 */
		void setArea(Area& area);
		void setArea(Area& area, const Coor& c, const Quat4& q);
		void setArea(Area& area, const Coor& c, const Euler3& a);

		void setArea(Area& area, const ViewPoint& vp);
		void setArea(Area& area, const SpawnPoint& sp);
		void resetArea();

		void setParent(PosNode& p) { parent_ = &p; }
		void resetParent();

		bool hasParent() const { return parent_ != 0; }
		PosNode* parent() { return parent_; }
		const PosNode* parent() const { return parent_; }

		/**
		 * The area we are presently in.
		 */
		const Area* area() const { return area_; }
		Area* area() { return area_; }


		/**
		 *
		 */
		bool hasArea() const { return area_ != 0; }

		short terrainStyle() const;
		long touchedTerrain() const;

		void worldViewPoint(ViewPoint& dest, bool doCalcNext = false, const PosNode* stopAt = 0) const;

		/**
		 * Used to check if the positions
		 * are in the same or in neighbouring
		 * areas. If not, keframe interpolation
		 * would be meaningless.
		 */
		bool isKeyFramePath(const Pos& other) const;


		/**
		 * Gets the face direction of the position. 0 is north.
		 * 1 is the next subdivision in a clockwise direction.
		 * This means east with 4 subdivisions and north_east
		 * with 8.
		 */
		short clockwiseFaceDirection(short subdivisions) const;


		/**
		 * Check if coor is in front of the pos.
		 * Return true if the coor is in front of the pos, and thus visible
		 * to the pos if the the landscape otherwise allows it.
		 *
		 * @param coor The Coor to check for.
		 * @return True if coor is in front, false otherwise.
		 */
		bool hasInFront(const Point3& coor) const;


		/**
		 * Calculate the eyeing angle.
		 * Calculetes the difference between the facing angle of this Pos
		 * and the angle towards the Point3 towards.
		 *
		 * @param towards The Coor that the Pos tries to see.
		 * @return The absolute minimum difference in braybrookians (giving possible values of 0-128)
		 */
		bray_t xzFaceAwayAngle(const Point3& towards) const;


		/**
		 * Get the radius of the position.
		 */
		inline coor_t radius() const { return radius_; }


		/**
		 * Set the radius of the position.
		 *
		 * @param r The new radius (as coor_t, fixed point decimal ).
		 */
		inline void setRadius(coor_t r) { radius_ = r; }


		/**
		 * Check if two positions are inside collition range.
		 * See if the sum of the radies are larger than the distance between
		 * the two positions.
		 *
		 * @param p The position to check against.
		 * @return true if inside collision range, false if not
		 */
		bool isInsideCollisionRange(const Pos& p) const;


		/**
		 * Check if two positions are within collision range.
		 * Check if the two positions are within collision range of each other,
		 * but use a collision square instead of a circle.
		 *
		 * @param p The position to check against.
		 */
		bool isInsideCollisionRangeLinf(const Pos& p) const;


		/**
		 * Is the Pos following the ground or is it floating in air?
		 *
		 * @return true if floating, false if on ground
		 */
		//inline bool hasOwnHeight() const { return index_ < 0; }


		/**
		 * Indicates that the pos is not somehow floating in air
		 * (climbin, falling, etc.)
		 */
		inline bool isGrounded() const { return isGrounded_; }

		/**
		 * Set wether the character is grounded. It is up to the physics
		 * object what they want to do with this state - typically they
		 * set the things y identically to the ground height.
		 */
		inline void setGrounded(bool state) { 
			isGrounded_ = state;
			if(isGrounded_)
				updateY();
		}

		inline void setIndex(short i) { index_ = i; }
		inline short index() const { return index_; }
		inline bool hasIndex() const { return index_ >= 0; }
		inline void setNoIndex() { index_ = -1; }


		/**
		 * Update y coordinate to reflect Area height and present Pos layer.
		 * Does nothing if the coordinate has no layer.
		 * If the Pos has a layer, updates the y coordinate to reflect the height of the area
		 * at the present layer, x and z coordinate.
		 *
		 * @param area The area to check height in.
		 */
		void updateY();

		coor_world_t pageDistanceSquared(const Pos& p) const;


		void freezeAtWorldViewPoint(const ViewPoint& vp);

	public:
		/** The area this position is inside */
		Area* area_;

		/** The parent of this position */
		PosNode* parent_;

		/** The radius of the position in coor_t units. */
		coor_t radius_;

		/** Area's id of the layer that this position is resting on. */
		short index_;

		bool isGrounded_;
	};

}

#endif
