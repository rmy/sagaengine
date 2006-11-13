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
#include "CollisionGrid.hpp"
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
#include "../physics/PhysicsSolverComponent.hpp"
#include <cstdio>


namespace se_core {
	Area
	::Area(String* name, coor_tile_t w, coor_tile_t h)
			: PosNode(got_AREA, name->get()), width_(w), height_(h)
			, multiSimObjects_(new MultiSimObject[ MGOA_COUNT ])
			, isActive_(false), pageX_(-1), pageY_(-1), pageZ_(-1)
			, factory_(0) {

		// Init to default position
		position_.reset();
		nextPosition_.reset();

		coor_t ySize = CoorT::fromTile(w);
		BoundingBox b;
		b.setMin(0, 0, 0);
		b.setMax(CoorT::fromTile(w), ySize, CoorT::fromTile(h));
		position_.setBounds(b);
		nextPosition_.setBounds(b);

		// Singly linked list containing all things in area. Listeners
		// my subscribe to get events when a members is removed
		// or added.
		allThings_ = new ReportingMultiSimObject(*this, RMGOA_ALL_THINGS);

		// Stored for destruction purposed.
		// TODO: fix this mess?
		nameString_ = name;

		// 3x3 array to hold self and neighbours
		for(short i = 0; i < 27; ++i) {
			neighbours_[i] = 0;
		}
		// Add self in center
		neighbours_[ 1 + 1 * 3 + 1 * 9 ] = this;

		physicsSolverComponent_ = new PhysicsSolverComponent(this);
		actionComponent_ = new ActionComponent(this);
		scriptComponent_ = new ScriptComponent(this, actionComponent_);
		// Register with area manager
		//SimSchema::areaManager.addArea(this);
	}


	Area
	::~Area() {
		delete[] multiSimObjects_;
		delete allThings_;
		delete nameString_;
		delete physicsSolverComponent_;
		delete scriptComponent_;
		delete actionComponent_;
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
		SimObjectList::iterator_type it = this->allThings().iterator();
		Thing* t;
		SimCompositeFactory* td;
		while(it != SimObjectList::end()) {
			t = SimSchema::simObjectList.nextThing(it);
			td = SimSchema::thingManager().factory(t->name());
			DebugExec(const Point3& c = t->pos().localCoor());
			Dump((sprintf(log_msg(), "O %s %.2f %.2f", t->name(), CoorT::toFloat(c.x_), CoorT::toFloat(c.y_)), log_msg()));

			// Statistics
			/*
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
			*/


			Dump("/");
		}

		// End of file
		Dump("Q");
	}


	MultiSimObject& Area
	::allThings() const {
		return *allThings_;
	}


	ReportingMultiSimObject& Area
	::reportingThings() {
		return *allThings_;
	}


	//MultiSimObject& Area
	//::movingThings() {
	//	return multiSimObjects_[ MGOA_MOVING_THINGS ];
	//}


	void Area
	::addPosNode(PosNode& posNode) {
		if(posNode.isType(got_THING)) {
			Thing* t = static_cast<Thing*>(&posNode);
			addThing(*t);
		}
	}


	void Area
	::removePosNode(PosNode& posNode) {
		if(posNode.isType(got_THING)) {
			Thing* t = static_cast<Thing*>(&posNode);
			removeThing(*t);
		}
	}


	void Area
	::addThing(Thing& thing) {
		//LogMsg(thing.name());
		allThings_->add(thing);

		// Things area added from Actor::flip(), during
		// changeArea, just before the nextPos() is
		// flipped into pos

		if(thing.isPickable())
			multiSimObjects_[ MGOA_PICKABLE_THINGS ].add(thing);

		if(thing.isMover())
			multiSimObjects_[ MGOA_MOVING_THINGS ].add(thing);

		if(thing.isCollideable()) {
			physicsSolverComponent_->addCollideable(thing);
			/*
			if(isActive_) {
				// TODO: Should use speed + radius
				coor_t speedAndRadius = thing.nextPos().radius() + MAX_SPEED;
				collisionGrid_->insert(thing.nextPos().worldCoor(), speedAndRadius, thing);
			}
			multiSimObjects_[ MGOA_PUSHABLE_THINGS ].add(thing);
			*/
		}

		//if(thing.isPusher())
		//	multiSimObjects_[ MGOA_PUSHING_THINGS ].add(thing);

		if(!thing.isType(got_ACTOR))
			multiSimObjects_[ MGOA_ACTORS ].add(thing);
	}


	void Area
	::removeThing(Thing& thing) {
		allThings_->remove(thing);
		/*
		if(isActive_ && thing.isCollideable()) {
			// TODO: Should use speed + radius
			coor_t speedAndRadius = thing.pos().radius() + MAX_SPEED;
			bool didDelete = collisionGrid_->remove(thing.pos().worldCoor(), speedAndRadius, thing);
			if(!didDelete) {
				coor_t speedAndRadius = thing.nextPos().radius() + MAX_SPEED;
				didDelete = collisionGrid_->remove(thing.nextPos().worldCoor(), speedAndRadius, thing);
				if(!didDelete) LogMsg("Couldn't remove " << thing.name() << " from collision grid");
			}
			//LogMsg(thing.name());
			// TODO: This assert fails?
			DbgAssert(didDelete);
		}
		*/
		if(thing.isCollideable()) {
			physicsSolverComponent_->removeCollideable(thing);
		}

		multiSimObjects_[ MGOA_MOVING_THINGS ].remove(thing);
		multiSimObjects_[ MGOA_PICKABLE_THINGS ].remove(thing);
		//multiSimObjects_[ MGOA_PUSHABLE_THINGS ].remove(thing);
		//multiSimObjects_[ MGOA_PUSHING_THINGS ].remove(thing);
		multiSimObjects_[ MGOA_ACTORS ].remove(thing);
	}


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
				/*
				if(pos.hasInFront(a->pos().localCoor()) && actor.findRunnableCutscene(*a)) {
					nearest = coor.xzDistanceSquared(a->pos().localCoor());
					actor.setTarget(a);
					actor.setPickTarget(a);
				}
				*/
			}
		}
		//TODO:
		//if(actor.hasTarget()) {
		//	actor.setDefaultAction(actionStartCutscene);
		//}

		return actor.pickTarget();
	}


	CollisionGrid* Area
	::collisionGrid() {
		return physicsSolverComponent_->collisionGrid(); 
	}


	void Area
	::setActive(bool state) {
		if(state == isActive_) return;
		isActive_ = state;
		physicsSolverComponent_->setActive(state);

		SimObjectList::iterator_type it = allThings_->iterator();
		while(it != SimObjectList::end()) {
			Thing* t = SimSchema::simObjectList.nextThing(it);
			if(t->isType(got_ACTOR)) {
				Actor* a = reinterpret_cast<Actor*>(t);
				a->setActive(isActive_);
			}
		}
	}


	void Area
	::enter(Actor& performer) {
		Cutscene* c = 0;

		SimObjectList::iterator_type it = multiSimObject(MGOA_CUTSCENES).iterator();
		while(it != SimObjectList::end()) {
			c = SimSchema::simObjectList.nextCutscene(it);
			Assert(c != 0);
			if(!performer.questGoals().contains(c->afterTheseGoals())) {
				c = 0;
				continue;
			}
			if(performer.questGoals().sharesAny(c->beforeTheseGoals())) {
				c = 0;
				continue;
			}
			break;
		}
		if(!c) return;
		if(c->questGoal()) {
			performer.questGoals().add(*(c->questGoal()));
		}
		// TODO:
		//c->setScripts(*this, performer);
	}


	void Area
	::reset() {
		// Get newly spawned objects into allThings()
		flipSpawns();

		// Shedule all things for destruction, and flip
		// it out of area
		SimObjectList::iterator_type it = allThings().iterator();
		while(it != SimObjectList::end()) {
			Thing* t = SimSchema::simObjectList.nextThing(it);
			// Put thing into ThingFactories destruction queue
			//LogMsg("Destroy " << name() << ": " <<t->name());
			t->scheduleForDestruction();
			// Flip thing out of area
			t->flip();
		}

		it = multiSimObjects_[ MGOA_SPAWNS ].iterator();
		while(it != SimObjectList::end()) {
			// Spawned things doesn't have to be actors
			Thing* t = SimSchema::simObjectList.nextThing(it);
			//LogMsg("Destroy spawn " << name() << ": " << t->name());

			delete t;
			//t->scheduleForDestruction();
			//t->flip();
		}
		// New spawn are no longer new spawns once they are flipped
		multiSimObjects_[ MGOA_SPAWNS ].clear();
	}


	bool Area
	::isNeighbour(Area& area) {
		if(pageX_ < 0 || pageY_ < 0 || pageZ_ < 0)
			return false;
		if(area.pageX_ < 0 || pageY_ < 0 || area.pageZ_ < 0)
			return false;

		short relX = area.pageX_ - pageX_;
		short relY = area.pageY_ - pageY_;
		short relZ = area.pageZ_ - pageZ_;
		if(relX < -1 || relX > 1) return false;
		if(relY < -1 || relY > 1) return false;
		if(relZ < -1 || relZ > 1) return false;

		return true;
	}


	bool Area
	::addNeighbour(Area* area) {
		if(pageX_ < 0 || pageY_ < 0 || pageZ_ < 0)
			return false;
		if(area->pageX_ < 0  || area->pageY_ < 0 || area->pageZ_ < 0)
			return false;

		short relX = area->pageX_ - pageX_;
		short relY = area->pageY_ - pageY_;
		short relZ = area->pageZ_ - pageZ_;
		if(relX < -1 || relX > 1) return false;
		if(relY < -1 || relY > 1) return false;
		if(relZ < -1 || relZ > 1) return false;

		Assert(neighbours_[(relX + 1) + 3 * (relY + 1) + 9 * (relZ + 1)] == 0);

		neighbours_[ (relX + 1) + 3 * (relY + 1) + 9 * (relZ + 1) ] = area;
		return true;
	}


	Area* Area
	::neighbour(short relX, short relY, short relZ) {
		if(relX < -1 || relX > 1 || relY < -1 || relY > 1 || relZ < -1 || relZ > 1) {
			// Not direct neighbour, recurse to correct area
			short rx = (relX < 0) ? -1 : (relX > 0) ? 1 : 0;
			short ry = (relY < 0) ? -1 : (relY > 0) ? 1 : 0;
			short rz = (relZ < 0) ? -1 : (relZ > 0) ? 1 : 0;

			Area* a = neighbours_[ (rx + 1) + (ry + 1) * 3 + (rz + 1) * 9 ];
			if(!a) return 0;
			return a->neighbour(relX - rx, relY - ry, relZ - rz);
		}
		// Direct neighbour
		return neighbours_[ (relX + 1) + (relY + 1) * 3 + (relZ + 1) * 9 ];
	}


	Area* Area
	::neighbour(const Point3& worldCoor) {
		coor_t x = worldCoor.x_ - nextPos().worldCoor().x_;
		coor_t y = worldCoor.y_ - nextPos().worldCoor().y_;
		coor_t z = worldCoor.z_ - nextPos().worldCoor().z_;

		int relX = 0;
		if(x < 0) relX = -1;
		if(CoorT::tile(x) >= width()) relX = 1;

		int relY = 0;
		//if(y < 0) relY = -1;
		//if(CoorT::tile(y) >= width()) relY = 1;
	
		int relZ = 0;
		if(z < 0) relZ = -1;
		if(CoorT::tile(z) >= height()) relZ = 1;
	
		return neighbour(relX, relY, relZ);
	}


	void Area
	::flipSpawns(void) {
		// Flip new spawns into area
		SimObjectList::iterator_type it = multiSimObjects_[ MGOA_SPAWNS ].iterator();
		while(it != SimObjectList::end()) {
			// Spawned things doesn't have to be actors
			Thing* t = SimSchema::simObjectList.nextThing(it);
			//LogMsg(t->pos().y_);
			//LogMsg(t->name());

			// Newly spawned things will change area on first flip.
			// This will call Area::addThing, inserting it into
			// collisionGrid_ and other structures where it belong
			t->flip();
		}
		// New spawn are no longer new spawns once they are flipped
		multiSimObjects_[ MGOA_SPAWNS ].clear();
	}



	Thing* Area
	::spawn(const char* thingName, const ViewPoint& vp, long deniedTsMask, PosNode* parent) {
		if(deniedTsMask != 0 && (tsMask(terrainStyle(vp.coor_)) & deniedTsMask) != 0) {
			// Tried to spawn on denied terrain type
			LogMsg((int)(tsMask(terrainStyle(vp.coor_))));
			return 0;
		}

		// Create the thing
		Thing* thing = SimSchema::thingManager().create(thingName);

		// Set position and direction
		thing->nextPos().setArea(*this, vp);

		if(parent) {
			thing->nextPos().setParent(*parent);
		}

		// Initial index, if area type is using it
		thing->nextPos().updateWorldViewPoint();
		thing->nextPos().setIndex( index(thing->nextPos().worldCoor()) );

		// Add the thing to the list of new spawns
		multiSimObjects_[ MGOA_SPAWNS ].add(*thing);

		//LogMsg("Spawned " << thingName << " in " << name());

		// Return the newly created thing
		return thing;
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
