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
		void setXZ(const Pos& original);

		inline void setLocalCoor(Point3& original) {
			localCoor().set(original);
		}

		inline const Point3& worldCoor() const { return world_.coor_; }
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
		 * Area.
		 */
		void setArea(Area& area, bool doKeepWorldCoor = false);
		void setArea(Area& area, const Point3& c, const Quat4& q);
		void setArea(Area& area, const Point3& c, const Euler3& a);

		void setArea(Area& area, const ViewPoint& vp);
		void resetArea();

		void setParent(PosNode& p) { parent_ = &p; }
		void setParent(PosNode& p, bool doKeepWorldCoor);
		void resetParent();

		bool hasParent() const { return parent_ != 0; }
		bool didParentMove() const;
		PosNode* parent() { return parent_; }
		const PosNode* parent() const { return parent_; }

		/**
		 * Update the world_ viewpoint according to the 
		 * parents nextPos(). The parents nextPos().world_
		 * must already be updated.
		 */
		void updateWorldViewPoint();
		void updateLocalViewPoint();


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

		void reset() {
			area_ = 0;
			parent_ = 0;
			local_.setIdentity();
			world_.setIdentity();
			radius_ = 0;
			index_ = -1;
			isGrounded_ = false;
		}

		void freezeAtWorldViewPoint();

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