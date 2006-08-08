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


#ifndef sim_area_Area_hpp
#define sim_area_Area_hpp

#include "sim_area.hpp"
#include "../SimObject.hpp"
#include "../PosNode.hpp"
#include "../action/sim_action.hpp"
#include "../stat/sim_stat.hpp"
#include "../stat/MultiSimObject.hpp"
#include "../thing/sim_thing.hpp"
#include "util/type/util_type.hpp"
#include "util/math/CoorT.hpp"
#include "util/vecmath/util_vecmath.hpp"
#include "sim/config/sim_config.hpp"


namespace se_core {

	class Area : public PosNode {
	public:
		enum MultiSimObjectType {
			MGOA_PICKABLE_THINGS = 0,
			MGOA_MOVING_THINGS,
			MGOA_PUSHABLE_THINGS,
			MGOA_PUSHING_THINGS,
			MGOA_ACTORS,
			MGOA_CUTSCENES,
			MGOA_SPAWNS,
			MGOA_COUNT
		};

		enum ReportingMultiSimObjectType {
			RMGOA_ALL_THINGS = 0,
			RMGOA_COUNT
		};

		/**
		 * Create a new area of a given size.
		 */
		Area(String* name, coor_tile_t w, coor_tile_t h);

		/**
		 * Destructor.
		 */
		virtual ~Area();

		/**
		 * Return true if parameter is got_AREA.
		 * Inherited from GameObject class.
		 */
		virtual bool isType(enum SimObjectType type) {
			if(type == got_AREA) return true;
			return false;
		}

		/**
		 * Height of ground (y-coordinate) at given (coor.x_, coor.z_).
		 */
		virtual coor_t groundHeight(const Point3& coor, short index = -1) const = 0;

		/**
		 * Some area types maintain an index with positions.
		 */
		virtual short index(const Point3& wc, short oldIndex = -1) const = 0;

		/**
		 * Sets the grid used to speed up the collisions detection.
		 * Often there an area is inactive much of the time, and a fine grained grid
		 * may take up quite a lot of memory. So the grid is passed around from
		 * one active area to the next.
		 */
		void setCollisionGrid(CollisionGrid* grid);

		/**
		 * Resets collision grid.
		 * @see setCollisionGrid
		 */
		CollisionGrid* resetCollisionGrid() {
			CollisionGrid* g = collisionGrid_;
			collisionGrid_ = 0;
			return g;
		}

		/**
		 * Save area to a stream.
		 * @note: NOT YET IMPLEMENTED.
		 */
		void saveThings(/* stream */);

		const AreaFactory* factory() { return factory_; }
		void setFactory(const AreaFactory* f) { factory_ = f; }

	public:
		virtual void force(const Point3& coor, Force& dest) const = 0;
		coor_tile_t width() const { return width_; }
		coor_tile_t height() const { return height_; }

		void setPage(short x, short y, short z) { pageX_ = x; pageY_ = y; pageZ_ = z; }
		short pageX() const { if(pageX_ < 0) return 0; return pageX_; }
		short pageY() const { if(pageY_ < 0) return 0; return pageY_; }
		short pageZ() const { if(pageZ_ < 0) return 0; return pageZ_; }

		coor_world_t pagePosX() const {
			//TODO: Fixed point overflows possible
			return pos().localCoor().x_;
		}
		coor_world_t pagePosZ() const {
			//TODO: Fixed point overflows possible
			return pos().localCoor().z_;
		}

		MultiSimObject& multiSimObject(int type) { return multiSimObjects_[ type ]; }
		MultiSimObject& allThings();
		ReportingMultiSimObject& reportingThings();
		MultiSimObject& movingThings();
		MultiSimObject& pushableThings();
		MultiSimObject& pushingThings();

		void addThing(Thing& thing);
		void removeThing(Thing& thing);
		void addPosNode(PosNode& thing);
		void removePosNode(PosNode& thing);
		bool isLegalCoor(coor_tile_t x, coor_tile_t y) const {
			return (x >= 0 && y >= 0 && x < width_ && y < height_);
		}

		bool isLegalCoor(const Point3& worldCoor) const {
			return (worldCoor.x_ >= nextPosition_.localCoor().x_
					&& worldCoor.z_ >= nextPosition_.localCoor().z_
					&& worldCoor.xTile() < nextPosition_.localCoor().xTile() + width_
					&& worldCoor.zTile() < nextPosition_.localCoor().zTile() + height_);

		}

		virtual short terrainStyle(const Point3& coor, short index = -1) const = 0;
		virtual short nextTerrainStyle(bray_t direction, const Point3& coor) = 0;
		static inline long tsMask(short ts) { return (1L << ts); }

		/// Wall collision utility method
		virtual long touchedTerrain(const Point3& centre, coor_t radius) const;
		/// Line of sight utility method
		virtual long touchedTerrain(const Point3& from, const Point3& to) const;

		Thing* findPickTarget(Player& actor);
		Thing* findDefaultActionTarget(Player& actor);
		bool isActive() const { return isActive_; }
		void enter(Actor& performer);
		void reset();

		/**
		 * Get neighbour.
		 * @param relX
		 * @param relY
		 */
		Area* neighbour(short relX, short relY, short relZ);

		Area* neighbour(const Point3& worldCoor);

		/**
		 * Check if the area passed in is a neighbour, and if so,
		 * register it as such.
		 * @return true if the passed in area is a neighbour
		 */
		bool addNeighbour(Area* area);
		bool isNeighbour(Area& area);

		void testActors2ThingsCollisions(); //Actor** movers, short moverCount);

		/**
		 * Flip all moving things in this area, making
		 * their nextPos() their pos(). This method
		 * is called on active areas by the SimEngine
		 * class in the beginning of each step.
		 */
		void flipChildren(void);

		/**
		 * Call affect on all moving things, then calculate
		 * the new nextPos(). This new nextPos() should
		 * later be tested for collisions and may be reverted.
		 */
		//int performPhysics(Actor** movers);
		int performChildPhysics(Actor** movers);


		/**
		 * Spawn a new thing in this area. Spawning should be done
		 * this way, because a thing not belonging to an area will
		 * never be flip()'ed.
		 */
		Thing* spawn(const char* thingName, const ViewPoint& coor, long deniedTsMask = 0, PosNode* parent = 0);


	private:
		/**
		 * List of movers this step. Updated by the coordinate precalcer and
		 * used by the collision detector.
		 */
		Actor** movers_;

		/** Number of movers presently in the movers_ array */
		short moverCount_;
		

	protected:
		friend class AreaManager;

		void setActive(bool state);

	protected:
		coor_tile_t width_, height_;
		bool isActive_;
		int pageX_, pageY_, pageZ_;

		String* nameString_; // For proper destruction of content only

		MultiSimObject* multiSimObjects_;
		ReportingMultiSimObject* allThings_;
		Area* neighbours_[ 3 * 3 * 3 ];
		CollisionGrid* collisionGrid_;

		const AreaFactory* factory_;
	};

}

#endif
