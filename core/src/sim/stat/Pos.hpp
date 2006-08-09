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

#include "util/vecmath/ViewPoint.hpp"
#include "util/vecmath/Point3.hpp"
#include "../sim.hpp"
#include "../area/sim_area.hpp"
#include "../config/sim_config.hpp"


namespace se_core {
	/**
	 * Position, movement and volume info.
	 * The Pos class maintains coordinates, volume and movement info for Thing
	 * and subclasses.
	 */
	class Pos {
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

		/**
		 * Copy x and z coordinates from a source.
		 * Both local and world coordinates are copied. This is useful
		 * for resetting movement in the xz-plane.
		 */
		void setXZ(const Pos& original);

		/**
		 * Set local coordinate.
		 * @param original the new coordinate.
		 */
		inline void setLocalCoor(Point3& original) {
			localCoor().set(original);
		}

		/**
		 * Return world coordinate.
		 * @return a reference to the world coordinate of the position
		 */
		inline const Point3& worldCoor() const { return world_.coor_; }

		/**
		 * Return world coordinate.
		 * @return a reference to the world coordinate of the position
		 */
		inline Point3& worldCoor() { return world_.coor_; }
		inline const ViewPoint& worldViewPoint() const { return world_; }
		inline ViewPoint& worldViewPoint() { return world_; }

		#ifdef SE_QUAT
		const Quat4& worldFace() const { return world_.face_; }
		Quat4& worldFace() { return world_.face_; }
		#else
		const Euler3& worldFace() const { return world_.face_; }
		Euler3& worldFace() { return world_.face_; }
		#endif


		inline const Point3& localCoor() const { return local_.coor_; }
		inline Point3& localCoor() { return local_.coor_; }
		inline const ViewPoint& localViewPoint() const { return local_; }
		inline ViewPoint& localViewPoint() { return local_; }

		#ifdef SE_QUAT
		const Quat4& localFace() const { return local_.face_; }
		Quat4& localFace() { return local_.face_; }
		#else
		const Euler3& localFace() const { return local_.face_; }
		Euler3& localFace() { return local_.face_; }
		#endif


		inline bool localEquals(ViewPoint& vp) {
			return local_.viewPointEquals(vp);
		}

		inline bool localEquals(Pos& p) {
			return local_.viewPointEquals(p.local_);
		}

		inline bool worldEquals(ViewPoint& vp) {
			return world_.viewPointEquals(vp);
		}

		inline bool worldEquals(Pos& p) {
			return world_.viewPointEquals(p.world_);
		}


		/**
		 * Set new area.
		 * @param area new area of position.
		 * @param doKeepWorldCoor wether the local viewpoint should be updated
		 *   to maintain the world coordinate
		 */
		void setArea(Area& area, bool doKeepWorldCoor = false);

		/**
		 * Set area and viewpoint.
		 * @param area new area
		 * @param vp new viewpoint
		 * @param isLocalViewPoint wether vp is a viewpoint in local space or world space
		 */
		void setArea(Area& area, const ViewPoint& vp, bool isLocalViewPoint = true);

		/**
		 * Leave any area.
		 */
		void resetArea();

		/**
		 * Set parent.
		 * @param p the new parent.
		 * @param doKeepWorldCoor wether the local viewpoint should be updated
		 *   to maintain the world viewpoint
		 */
		void setParent(PosNode& p, bool doKeepWorldCoor = false);

		/**
		 * Set parent to none.
		 * @param doKeepWorldCoor wether the local viewpoint should be updated
		 *   to maintain the world viewpoint
		 */
		void resetParent(bool doKeepWorldCoor = true);

		/**
		 * Does the Pos have a parent?
		 */
		bool hasParent() const { return parent_ != 0; }

		/**
		 * Did the parent move last tick?
		 * If the parent moved, the world coordinate of this
		 * position has changed. The engine uses this method
		 * to check wether it needs to update the world viewpoint.
		 */
		bool didParentMove() const;

		/**
		 * Get parent.
		 */
		PosNode* parent() { return parent_; }
		/**
		 * Get parent.
		 * const version.
		 */
		const PosNode* parent() const { return parent_; }

		/**
		 * Update the world_ viewpoint according to the 
		 * parents nextPos(). The parents nextPos().world_
		 * must already be updated.
		 */
		void updateWorldViewPoint();

		/**
		 * Update the local_ viewpoint according to
		 * the world_ viewpoint and the parents nextPos().
		 */
		void updateLocalViewPoint();

		/**
		 * The area the Pos is presently in.
		 */
		Area* area() { return area_; }

		/**
		 * The area the Pos is presently in.
		 * const version.
		 */
		const Area* area() const { return area_; }


		/**
		 * Does the Pos have an area?
		 */
		bool hasArea() const { return area_ != 0; }

		/**
		 * The terrain style at the present position.
		 * The request is passed on to the area the Pos is within.
		 * If the Pos does not have an area, TS_VOID is returned.
		 */
		short terrainStyle() const;

		/**
		 * The terrain styles at the present position and nearby.
		 * The request is passed on to the area the Pos is within.
		 * If the Pos does not have an area, TSM_VOID is returned.
		 */
		long touchedTerrain() const;

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

		/**
		 * Set index of the position.
		 * The meaning of this value is at the discretion of the Area
		 * the Pos is inside.
		 */
		inline void setIndex(short i) { index_ = i; }
		
		/**
		 * Get the index of the Pos.
		 * The meaning of this value is at the discretion of the Area
		 * the Pos is inside. -1 does mean that the index is not in
		 * in use.
		 */
		inline short index() const { return index_; }

		/**
		 * Does the Pos have an index?
		 */
		inline bool hasIndex() const { return index_ >= 0; }

		/**
		 * Set index to none (-1).
		 */
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

		// Deprecated as the result of world_
		//coor_world_t pageDistanceSquared(const Pos& p) const;

		void reset() {
			area_ = 0;
			parent_ = 0;
			local_.setIdentity();
			world_.setIdentity();
			radius_ = 0;
			index_ = -1;
			isGrounded_ = false;
		}

		/**
		 * Reset parent while maintaining world viewpoint.
		 */
		//void freezeAtWorldViewPoint();

	public:
		/** The area this position is inside */
		Area* area_;

		/** The parent of this position */
		PosNode* parent_;

		/** The position in local coordinates */
		ViewPoint local_;

		/** The position in world coordinates */
		ViewPoint world_;

		/** The radius of the position in coor_t units. */
		coor_t radius_;

		/** Area's id of the layer that this position is resting on. */
		short index_;

		bool isGrounded_;
	};

}

#endif
