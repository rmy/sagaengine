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


		Area(String* name, coor_tile_t w, coor_tile_t h);
		virtual ~Area();

		virtual bool isType(enum SimObjectType type) {
			if(type == got_AREA) return true;
			return false;
		}

		virtual coor_t groundHeight(const Coor& coor, short index = -1) const = 0;
		virtual void updateIndex(Pos& pos) const = 0;
		void setCollisionGrid(CollisionGrid* grid);
		CollisionGrid* resetCollisionGrid() {
			CollisionGrid* g = collisionGrid_;
			collisionGrid_ = 0;
			return g;
		}
		void saveThings(/* stream */);


	public:
		virtual void force(const Coor& coor, Force& dest) const = 0;
		coor_tile_t width() const { return width_; }
		coor_tile_t height() const { return height_; }

		void setPage(short x, short z) { pageX_ = x; pageZ_ = z; }
		short pageX() const { if(pageX_ < 0) return 0; return pageX_; }
		short pageZ() const { if(pageZ_ < 0) return 0; return pageZ_; }

		coor_world_t pagePosX() const {
			//TODO: Owerflows possible
			return pos().coor().x_;
		}
		coor_world_t pagePosZ() const {
			//TODO: Owerflows possible
			return pos().coor().y_;
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
		bool isLegalCoor(int x, int y) const { return (x >= 0 && y >= 0 && x < width_ && y < height_); }

		virtual short terrainStyle(const Coor& coor, short index = -1) const = 0;
		virtual short nextTerrainStyle(bray_t direction, const Coor& coor) = 0;
		static inline long tsMask(short ts) { return (1L << ts); }

		/// Wall collision utility method
		virtual long touchedTerrain(const Coor& centre, coor_t radius) const;
		/// Line of sight utility method
		virtual long touchedTerrain(const Coor& from, const Coor& to) const;

		/*
		void setEntrance(short id, float x, float y);
		bool hasEntrance(short id) { return entrances_[id] != 0; }
		Coor* entrance(short id);
		*/
		Thing* findPickTarget(Player& actor);
		Thing* findDefaultActionTarget(Player& actor);
		bool isActive() { return isActive_; }
		void enter(Actor& performer);
		void reset();

		//static const int MAX_ENTRANCES = 10;

		/**
		 * Get neighbour.
		 * @param relX
		 * @param relY
		 */
		Area* getNeighbour(short relX, short relZ);

		/**
		 * Check if the area passed in is a neighbour, and if so,
		 * register it as such.
		 * @return true if the passed in area is a neighbour
		 */
		bool addNeighbour(Area* area);
		bool isNeighbour(Area& area);

		void testActors2ThingsCollisions(Actor** movers, short moverCount);

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
		int performPhysics(Actor** movers);
		int performChildPhysics(Actor** movers);


		/**
		 * Spawn a new thing in this area. Spawning should be done
		 * this way, because a thing not belonging to an area will
		 * never be flip()'ed.
		 */
		Thing* spawn(const char* thingName, const Coor& coor, const Quat4& face);

	protected:
		friend class AreaManager;

		void setActive(bool state);

	protected:
		coor_tile_t width_, height_;

		String* nameString_; // For proper destruction of content only
		//Coor** entrances_;

		MultiSimObject* multiSimObjects_;
		ReportingMultiSimObject* allThings_;
		bool isActive_;


		short pageX_, pageZ_;

		Area* neighbours_[9];
		CollisionGrid* collisionGrid_;
	};

}

#endif
