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
#include "comp/Component.hpp"
#include "comp/ComponentFactory.hpp"
#include "../sim.hpp"
#include "../action/sim_action.hpp"
#include "../stat/sim_stat.hpp"
#include "../thing/sim_thing.hpp"
#include "util/type/util_type.hpp"
#include "util/math/CoorT.hpp"
#include "util/bounds/BoundingBox.hpp"
#include "util/vecmath/util_vecmath.hpp"
#include "sim/config/sim_config.hpp"
#include "../pos/PosComponent.hpp"
#include "../physics/sim_physics.hpp"
#include "../spawn/sim_spawn.hpp"
#include "../script/sim_script.hpp"
#include "../react/sim_react.hpp"
#include "../signal/sim_signal.hpp"
#include "../zone/sim_zone.hpp"


namespace se_core {

	class _SeCoreExport Area : public Component {
	public:
		typedef ComponentPtr<Area, sct_BLOB> Ptr;

		/**
		 * Create a new area of a given size.
		 */
		Area(Composite* owner, const ComponentFactory* factory, String* name, coor_tile_t w, coor_tile_t h);

		/**
		 * Destructor.
		 */
		virtual ~Area();

		/**
		 * Height of ground (y-coordinate) at given (coor.x_, coor.z_).
		 */
		virtual coor_t groundHeight(const Point3& coor, short index = -1) const = 0;

		/**
		 * Some area types maintain an index with positions.
		 */
		virtual short index(const Point3& wc, short oldIndex = -1) const = 0;

		virtual bool doesTouchVoid(const Point3& wc, short index, coor_t radius) const = 0;

		/**
		 * Save area to a stream.
		 * @todo: NOT YET IMPLEMENTED.
		 */
		void saveThings(/* stream */);


	public:
		virtual void force(const Point3& coor, Vector3& dest) const = 0;
		coor_tile_t width() const { return width_; }
		coor_tile_t height() const { return height_; }

		coor_world_t pagePosX() const {
			//TODO: Fixed point overflows possible
			return posComponent_->pos().localCoor().x_;
		}
		coor_world_t pagePosZ() const {
			//TODO: Fixed point overflows possible
			return posComponent_->pos().localCoor().z_;
		}

		bool isLegalCoor(coor_tile_t x, coor_tile_t y) const {
			return (x >= 0 && y >= 0 && x < width_ && y < height_);
		}

		bool isLegalLocalCoor(const Point3& localCoor) const {
			//return pos().bounds_.hasInside(worldCoor);
			const Point3& c = posComponent_->nextPos().localCoor();
			return (localCoor.x_ >= c.x_
					&& localCoor.z_ >= c.z_
					&& localCoor.xTile() < c.xTile() + width_
					&& localCoor.zTile() < c.zTile() + height_);
		}

		bool isLegalWorldCoor(const Point3& worldCoor) const {
			const Point3& c = posComponent_->nextPos().worldCoor();
			return (worldCoor.x_ >= c.x_
					&& worldCoor.z_ >= c.z_
					&& worldCoor.xTile() < c.xTile() + width_
					&& worldCoor.zTile() < c.zTile() + height_);
		}

		virtual short terrainStyle(const Point3& coor, short index = -1) const = 0;
		virtual short nextTerrainStyle(bray_t direction, const Point3& coor) = 0;
		static inline long tsMask(short ts) { return (1L << ts); }

		/// Wall collision utility method
		virtual long touchedTerrain(const Point3& centre, coor_t radius) const;
		/// Line of sight utility method
		virtual long touchedTerrain(const Point3& from, const Point3& to) const;

		//Thing* findPickTarget(Player& actor) const;
		//Thing* findDefaultActionTarget(Player& actor) const;
		void reset();

		/**
		 * Get neighbour.
		 * @param relX
		 * @param relY
		 */
		Area* neighbour(short relX, short relY, short relZ);
		const Area* neighbour(short relX, short relY, short relZ) const;

		Area* neighbour(const Point3& worldCoor);
		const Area* neighbour(const Point3& worldCoor) const;

		/**
		 * Check if the area passed in is a neighbour, and if so,
		 * register it as such.
		 * @return true if the passed in area is a neighbour
		 */
		bool addNeighbour(Area* area);
		virtual bool isNeighbour(const Area& area) const;

		/**
		 * Flip all moving things in this area, making
		 * their nextPos() their pos(). This method
		 * is called on active areas by the SimEngine
		 * class in the beginning of each step.
		 */
		void flipSpawns(void);

		/**
		 * Spawn a new thing in this area. Spawning should be done
		 * this way, because a thing not belonging to an area will
		 * never be flip()'ed.
		 */
		Composite* spawn(const char* thingName, const ViewPoint& coor, long deniedTsMask = 0, PosComponent* parent = 0);


	protected:
		coor_tile_t width_, height_;

		String* nameString_; // For proper destruction of content only

		PosComponent* posComponent_;
		SpawnAreaComponent* spawnAreaComponent_;
		ScriptComponent* scriptComponent_;
		ActionComponent* actionComponent_;
		CollisionAreaComponent* collisionAreaComponent_;
		PhysicsAreaComponent* physicsAreaComponent_;
		SignalAreaComponent* signalAreaComponent_;
		ZoneAreaComponent* zoneAreaComponent_;
	};

}

#endif
