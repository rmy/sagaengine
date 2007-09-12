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


#include "Area.hpp"
#include "AreaManager.hpp"
#include "util/error/all.hpp"
#include "util/math/all.hpp"
#include "util/system/util_system.hpp"
#include "util/type/all.hpp"
#include "util/vecmath/all.hpp"
#include "sim/all.hpp"
#include "sim/action/all.hpp"
#include "sim/config/all.hpp"
#include "sim/react/ThingCollide.hpp"
#include "sim/schema/all.hpp"
#include "sim/script/all.hpp"
#include "sim/stat/all.hpp"
#include "sim/thing/all.hpp"
#include "sim/pos/PosComponent.hpp"
#include "comp/list/CompositeList.hpp"
#include "comp/list/ComponentList.hpp"
#include "../physics/PhysicsAreaComponent.hpp"
#include "../react/CollisionAreaComponent.hpp"
#include "../signal/SignalAreaComponent.hpp"
#include "../spawn/SpawnAreaComponent.hpp"
#include "../spawn/SpawnManager.hpp"
#include "../zone/ZoneAreaComponent.hpp"

#include <cstdio>
#include <cstring>

#ifndef abs
#define abs(n) (((n) < 0) ? (-(n)) : (n))
#endif 

namespace se_core {
	Area
	::Area(Composite* owner, const ComponentFactory* factory, coor_tile_t w, coor_tile_t h)
			: Component(sct_BLOB, owner, factory), width_(w), height_(h) {
		LogDetail(owner->name() << " area created with size " << w << ", " << h);

		posComponent_ = new PosComponent(owner);

		// Init to default position
		owner_->setParent(RootComponent::inactiveRoot());

		posComponent_->nextPos().reset();
		coor_t ySize = CoorT::fromTile(w);
		BoundingBox b;
		// Must be below 0, because things that has gravity is constantly falling
		// Floor adjustments are often done after wall collision tests
		b.setMin(0, -ySize, 0);
		b.setMax(CoorT::fromTile(w), ySize, CoorT::fromTile(h));
		posComponent_->nextPos().setBounds(b);
		posComponent_->flip();

		spawnAreaComponent_ = new SpawnAreaComponent(owner);
		collisionAreaComponent_ = new CollisionAreaComponent(owner);
		AreaEdge& areaEdge = collisionAreaComponent_->areaEdge();
		areaEdge.addLink(Point2(0, 0), Point2(CoorT::fromTile(w), 0));
		areaEdge.addLink(Point2(CoorT::fromTile(w), 0), Point2(CoorT::fromTile(w), CoorT::fromTile(h)));
		areaEdge.addLink(Point2(CoorT::fromTile(w), CoorT::fromTile(h)), Point2(0, CoorT::fromTile(h)));
		areaEdge.addLink(Point2(0, CoorT::fromTile(h)), Point2(0, 0));

		physicsAreaComponent_ = new PhysicsAreaComponent(owner, collisionAreaComponent_);
		actionComponent_ = new ActionComponent(owner);
		scriptComponent_ = new ScriptComponent(owner, actionComponent_);
		signalAreaComponent_ = new SignalAreaComponent(owner);
		zoneAreaComponent_ = new ZoneAreaComponent(owner);
		// Register with area manager
		//SimSchema::areaManager.addArea(this);
	}


	Area
	::~Area() {
		LogDetail(owner()->name() << " area destroyed");
		delete zoneAreaComponent_;
		delete signalAreaComponent_;
		delete scriptComponent_;
		delete actionComponent_;
		delete physicsAreaComponent_;
		delete collisionAreaComponent_;
		delete spawnAreaComponent_;
		delete posComponent_;
	}


	void Area
	::saveThings(/* stream */) {
		// Header
		Dump("EB01");

		// Name
		Dump((sprintf(log_msg(), "N %s", this->name()), log_msg()));
		/*
		// Entrances
		for(int i = 0; i < MAX_ENTRANCES; ++i) {
			if(!this->entrances_[i])
				continue;
			DebugExec(Point3& e = *this->entrances_[i]);
			Dump((sprintf(log_msg(), "E %d %.2f %.2f", i, CoorT::toFloat(e.x_), CoorT::toFloat(e.y_)), log_msg()));
		}
		*/

		// Things
			/*
		SimObjectList::iterator_type it = this->allThings().iterator();
		Thing* t;
		CompositeFactory* td;
		while(it != SimObjectList::end()) {
			t = SimSchema::simObjectList.nextThing(it);
			td = SimSchema::thingManager().factory(t->name());
			DebugExec(const Point3& c = t->pos().localCoor());
			Dump((sprintf(log_msg(), "O %s %.2f %.2f", t->name(), CoorT::toFloat(c.x_), CoorT::toFloat(c.y_)), log_msg()));

			// Statistics
			for(int i = 0; i < SV_COUNT; ++i) {
				if(td->singleValue(i) == t->singleValue(i)) continue;
				Dump((sprintf(log_msg(), "S %d %d", i, t->singleValue(i)), log_msg()));
			}

			// Attributes
			for(int i = 0; i < ATT_COUNT; ++i) {
				// TODO: Assumes that attributes will never be assigned in definition, only on instantiation...
				if(t->attribute(i).get()[0] == '\0') continue;
				Dump((sprintf(log_msg(), "A %d %s", i, t->attribute(i).get()), log_msg()));
			}

			Dump("/");
		}

		// End of file
		Dump("Q");
			*/
	}


	Composite* Area
	::findTarget(const char* factoryName, const Point3& worldCoor) const {
		// Default to maximum pick range
		coor_double_t nearestDist = 0;
		Composite* nearest = 0;

		CompositeList::Iterator it(owner()->children());
		while(it.hasNext()) {
			Composite* t = &it.next();
			if(strcmp(t->name(), factoryName) == 0) {
				PosComponent::Ptr pos(*t);
				coor_double_t dist = worldCoor.distanceSquared(pos->nextPos().worldCoor());
				if(!nearest || dist < nearestDist) {
					nearest = t;
					nearestDist = dist;
				}
			}
		}

		return nearest;
	}

	/*
	Thing* Area
	::findPickTarget(Player& actor) const {
		actor.setPickTarget(0);
		const Point3& coor = actor.pos().localCoor();

		// Default to maximum pick range
		coor_t nearest = (3 * COOR_RES);

		SimObjectList::iterator_type it = multiSimObject(MGOA_PICKABLE_THINGS).iterator();
		while(it != SimObjectList::end()) {
			Thing* t = SimSchema::simObjectList.nextThing(it);
			if(coor.xzDistanceLinf(t->pos().localCoor()) < nearest) {
				nearest = coor.xzDistanceLinf(t->pos().localCoor());
				actor.setPickTarget(t);
			}
		}
		//if(actor.pickTarget()) {
		//	actor.setDefaultAction(actionPick);
		//}
		return actor.pickTarget();
	}


	Thing* Area
	::findDefaultActionTarget(Player& actor) const {
		actor.resetDefaultAction();
		if(findPickTarget(actor)) return actor.pickTarget();

		actor.setTarget(0);
		const Pos& pos = actor.pos();
		const Point3& coor = pos.localCoor();

		coor_double_t nearest = -1;

		SimObjectList::iterator_type it = multiSimObject(MGOA_ACTORS).iterator();
		while(it != SimObjectList::end()) {
			Actor* a = SimSchema::simObjectList.nextActor(it);
			if(a == &actor) continue;
			if(a->cutscenes().isEmpty()) continue;
			if(nearest < 0 || coor.xzDistanceSquared(a->pos().localCoor()) < nearest) {
				// TODO:
				//if(pos.hasInFront(a->pos().localCoor()) && actor.findRunnableCutscene(*a)) {
				//	nearest = coor.xzDistanceSquared(a->pos().localCoor());
				//	actor.setTarget(a);
				//	actor.setPickTarget(a);
				//}
			}
		}
		//TODO:
		//if(actor.hasTarget()) {
		//	actor.setDefaultAction(actionStartCutscene);
		//}

		return actor.pickTarget();
	}
	*/


	void Area
	::reset() {
		// Get newly spawned objects into allThings()
		flipSpawns();
		//spawnAreaComponent_->flipSpawns();
		//multiSimObjects_[ MGOA_SPAWNS ].clear();

		// Shedule all things for destruction, and flip
		// it out of area
		{
			CompositeList::TreeIterator it(owner()->children());
			while(it.hasNext()) {
				Composite* t = &it.next();
				t->scheduleForDestruction();
			}
		}
	}


	bool Area
	::isNeighbour(const Area& area) const {
		Page rel;
		zoneAreaComponent_->offset(*area.zoneAreaComponent_, rel);
		return rel.isNeighbourOffset() && (rel.x_ == 0 || rel.z_ == 0);
	}


	bool Area
	::addNeighbour(Area* area) {
		if(isNeighbour(*area)) {
			AssertWarning(area->isNeighbour(*this), name() << " isn't relinked from " << area->name());
			zoneAreaComponent_->addLink(*area->zoneAreaComponent_);

			int px = zoneAreaComponent_->page().x_;
			int pz = zoneAreaComponent_->page().z_;

			int npx = area->zoneAreaComponent_->page().x_;
			int npz = area->zoneAreaComponent_->page().z_;

			AreaEdge& areaEdge = collisionAreaComponent_->areaEdge();

			coor_t w = CoorT::fromTile(width_);
			coor_t h = CoorT::fromTile(height_);
			Assert(pz == npz || px == npx);
			if(pz > npz) {
				areaEdge.removeLink(Point2(0, 0), Point2(w, 0));
			}
			else if(px < npx) {
				areaEdge.removeLink(Point2(w, 0), Point2(w, h));
			}
			else if(pz < npz) {
				areaEdge.removeLink(Point2(w, h), Point2(0, h));
			}
			else if(px > npx) {
				areaEdge.removeLink(Point2(0, h), Point2(0, 0));
			}

		}

		return zoneAreaComponent_->addNeighbour(*area->zoneAreaComponent_);
	}


	Area* Area
	::neighbour(short relX, short relY, short relZ) {
		ZoneAreaComponent* c = zoneAreaComponent_->neighbour(relX, relY, relZ);
		if(!c) return 0;
		return Area::Ptr(*c);
	}


	const Area* Area
	::neighbour(short relX, short relY, short relZ) const {
		const ZoneAreaComponent* c = zoneAreaComponent_->neighbour(relX, relY, relZ);
		if(!c) return 0;
		return Area::Ptr(*c);
	}


	Area* Area
	::neighbour(const Point3& worldCoor) {
		const Point3& wc =  posComponent_->nextPos().worldCoor();
		coor_t x = worldCoor.x_ - wc.x_;
		coor_t y = worldCoor.y_ - wc.y_;
		coor_t z = worldCoor.z_ - wc.z_;

		int relX = 0;
		if(x < 0) relX = -1;
		if(CoorT::tile(x) >= width()) relX = 1;

		int relY = 0;
	
		int relZ = 0;
		if(z < 0) relZ = -1;
		if(CoorT::tile(z) >= height()) relZ = 1;
	
		return neighbour(relX, relY, relZ);
	}


	const Area* Area
	::neighbour(const Point3& worldCoor) const {
		const Point3& wc =  posComponent_->nextPos().worldCoor();
		coor_t x = worldCoor.x_ - wc.x_;
		coor_t y = worldCoor.y_ - wc.y_;
		coor_t z = worldCoor.z_ - wc.z_;

		int relX = 0;
		if(x < 0) relX = -1;
		if(CoorT::tile(x) >= width()) relX = 1;

		int relY = 0;
	
		int relZ = 0;
		if(z < 0) relZ = -1;
		if(CoorT::tile(z) >= height()) relZ = 1;
	
		return neighbour(relX, relY, relZ);
	}


	void Area
	::flipSpawns(void) {
		spawnAreaComponent_->flipSpawns();
	}



	Composite* Area
	::spawn(const char* thingName, const ViewPoint& vp, long deniedTsMask, PosComponent* parent) {
		if(deniedTsMask != 0 && (tsMask(terrainStyle(vp.coor_)) & deniedTsMask) != 0) {
			// Tried to spawn on denied terrain type
			LogDetail((int)(tsMask(terrainStyle(vp.coor_))));
			return 0;
		}

		return spawnAreaComponent_->spawn(thingName, vp, parent);
	}


	long Area
	::touchedTerrain(const Point3& centre, coor_t radius) const {
		long touched = 0;
		Vector3 v(radius, 0, radius);
		for(coor_t dx = radius; dx > 0; dx -= COOR_RES) {
			for(coor_t dz = radius; dz > 0; dz -= COOR_RES) {
				Point3 c;
				c.set(centre);
				c.add(dx, 0, dz);
				touched |= tsMask(terrainStyle(c));

				c.set(centre);
				c.add(-dx, 0, dz);
				touched |= tsMask(terrainStyle(c));

				c.set(centre);
				c.add(dx, 0, -dz);
				touched |= tsMask(terrainStyle(c));

				c.set(centre);
				c.add(-dx, 0, -dz);
				touched |= tsMask(terrainStyle(c));
			}
		}

		return touched;
	}


	long Area
	::touchedTerrain(const Point3& from, const Point3& to) const {
		long touched = 0;

		// Bresenham
		// TODO: Should be modified so that all tiles touching the
		// line is taken into account.

		// The difference between the x's
		int deltax = abs(to.xTile() - from.xTile());
		// The difference between the y's
		int deltay = abs(to.zTile() - from.zTile());
		// Start x off at the first pixel
		int x = from.xTile();
		// Start y off at the first pixel
		int y = from.zTile();

		int xinc1, xinc2;
		if (to.xTile() >= from.xTile()) {
			// The x-values are increasing
			xinc1 = xinc2 = 1;
		}
		else {
			// The x-values are decreasing
			xinc1 = xinc2 = -1;
		}

		int yinc1, yinc2;
		if (to.zTile() >= to.xTile()) {
			// The y-values are increasing
			yinc1 = yinc2 = 1;
		}
		else {
			// The y-values are decreasing
			yinc1 = yinc2 = -1;
		}

		int den, num, numadd, numpixels;
		// There is at least one x-value for every y-value
		if (deltax >= deltay) {
			// Don't change the x when numerator >= denominator
			xinc1 = 0;
			// Don't change the y for every iteration
			yinc2 = 0;
			den = deltax;
			num = deltax >> 1;
			numadd = deltay;
			// There are more x-values than y-values
			numpixels = deltax;
		}
		// There is at least one y-value for every x-value
		else {
			// Don't change the x for every iteration
			xinc2 = 0;
			// Don't change the y when numerator >= denominator
			yinc1 = 0;
			den = deltay;
			num = deltay >> 1;
			numadd = deltax;
			// There are more y-values than x-values
			numpixels = deltay;
		}

		for (int curpixel = 0; curpixel <= numpixels; curpixel++) {
			// "Draw" the current pixel
			touched |= tsMask(terrainStyle(Point3(CoorT::fromInt(x), 0, CoorT::fromInt(y))));
			// Increase the numerator by the top of the fraction
			num += numadd;
			// Check if numerator >= denominator
			if (num >= den) {
				// Calculate the new numerator value
				num -= den;
				// Change the x as appropriate
				x += xinc1;
				// Change the y as appropriate
				y += yinc1;
			}
			// Change the x as appropriate
			x += xinc2;
			// Change the y as appropriate
			y += yinc2;
		}

		return touched;
	}

}
