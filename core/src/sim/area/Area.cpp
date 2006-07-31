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
#include <cstdio>


namespace se_core {
	Area
	::Area(String* name, coor_tile_t w, coor_tile_t h)
			: PosNode(got_AREA, name->get()), moverCount_(0), width_(w), height_(h)
			, multiSimObjects_(new MultiSimObject[ MGOA_COUNT ])
			, isActive_(false), pageX_(-1), pageY_(-1), pageZ_(-1)
			, collisionGrid_(0), factory_(0) {

		// Init to default position
		position_.reset();
		nextPosition_.reset();

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

		// Register with area manager
		//SimSchema::areaManager.addArea(this);
	}


	Area
	::~Area() {
		delete[] multiSimObjects_;
		delete allThings_;
		delete nameString_;
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
			DebugExec(Coor& e = *this->entrances_[i]);
			Dump((sprintf(log_msg(), "E %d %.2f %.2f", i, CoorT::toFloat(e.x_), CoorT::toFloat(e.y_)), log_msg()));
		}
		*/

		// Things
		SimObjectList::iterator_type it = this->allThings().iterator();
		Thing* t;
		ThingFactory* td;
		while(it != SimObjectList::NULL_NODE) {
			t = SimSchema::simObjectList.nextThing(it);
			td = SimSchema::thingManager().factory(t->name());
			DebugExec(const Coor& c = t->pos().coor_);
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
	::allThings() {
		return *allThings_;
	}


	ReportingMultiSimObject& Area
	::reportingThings() {
		return *allThings_;
	}


	MultiSimObject& Area
	::pushableThings() {
		return multiSimObjects_[ MGOA_PUSHABLE_THINGS ];
	}


	MultiSimObject& Area
	::pushingThings() {
		return multiSimObjects_[ MGOA_PUSHING_THINGS ];
	}


	MultiSimObject& Area
	::movingThings() {
		return multiSimObjects_[ MGOA_MOVING_THINGS ];
	}


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
			if(isActive_) {
				// TODO: Should use speed + radius
				coor_t speedAndRadius = thing.nextPos().radius() + COOR_RES;
				collisionGrid_->insert(thing.nextPos().worldCoor(), speedAndRadius, thing);
			}
			multiSimObjects_[ MGOA_PUSHABLE_THINGS ].add(thing);
		}

		if(thing.isPusher())
			multiSimObjects_[ MGOA_PUSHING_THINGS ].add(thing);

		if(!thing.isType(got_ACTOR))
			multiSimObjects_[ MGOA_ACTORS ].add(thing);
	}


	void Area
	::removeThing(Thing& thing) {
		allThings_->remove(thing);
		//LogMsg(thing.name());
		if(isActive_ && thing.isCollideable()) {
			// TODO: Should use speed + radius
			coor_t speedAndRadius = thing.pos().radius() + COOR_RES;
			//bool didDelete =
			collisionGrid_->remove(thing.pos().worldCoor(), speedAndRadius, thing);
			//LogMsg(thing.name());
			// TODO: This assert fails?
			//DbgAssert(didDelete);
		}

		multiSimObjects_[ MGOA_MOVING_THINGS ].remove(thing);
		multiSimObjects_[ MGOA_PICKABLE_THINGS ].remove(thing);
		multiSimObjects_[ MGOA_PUSHABLE_THINGS ].remove(thing);
		multiSimObjects_[ MGOA_PUSHING_THINGS ].remove(thing);
		multiSimObjects_[ MGOA_ACTORS ].remove(thing);
	}


	Thing* Area
	::findPickTarget(Player& actor) {
		actor.setPickTarget(0);
		const Coor& coor = actor.pos().coor_;

		// Default to maximum pick range
		coor_t nearest = (3 * COOR_RES);

		SimObjectList::iterator_type it = multiSimObject(MGOA_PICKABLE_THINGS).iterator();
		while(it != SimObjectList::NULL_NODE) {
			Thing* t = SimSchema::simObjectList.nextThing(it);
			if(coor.xzDistanceLinf(t->pos().coor_) < nearest) {
				nearest = coor.xzDistanceLinf(t->pos().coor_);
				actor.setPickTarget(t);
			}
		}
		//if(actor.pickTarget()) {
		//	actor.setDefaultAction(actionPick);
		//}
		return actor.pickTarget();
	}


	Thing* Area
	::findDefaultActionTarget(Player& actor) {
		actor.resetDefaultAction();
		if(findPickTarget(actor)) return actor.pickTarget();

		actor.setTarget(0);
		const Pos& pos = actor.pos();
		const Coor& coor = pos.coor();

		coor_double_t nearest = -1;

		SimObjectList::iterator_type it = multiSimObject(MGOA_ACTORS).iterator();
		while(it != SimObjectList::NULL_NODE) {
			Actor* a = SimSchema::simObjectList.nextActor(it);
			if(a == &actor) continue;
			if(a->cutscenes().isEmpty()) continue;
			if(nearest < 0 || coor.xzDistanceSquared(a->pos().coor_) < nearest) {
				if(pos.hasInFront(a->pos().coor_) && actor.findRunnableCutscene(*a)) {
					nearest = coor.xzDistanceSquared(a->pos().coor_);
					actor.setTarget(a);
					actor.setPickTarget(a);
				}
			}
		}
		if(actor.hasTarget()) {
			actor.setDefaultAction(actionStartCutscene);
		}

		return actor.pickTarget();
	}


	void Area
	::setCollisionGrid(CollisionGrid* grid) {
		collisionGrid_ = grid;

		// Make sure the grid does not have any members
		// from previous usage
		collisionGrid_->clear();

		// Align the grid coordinate system with
		// this areas coordinate system
		collisionGrid_->setSize(width_, height_);

		collisionGrid_->setOffset(position_.world_.coor_);

		// Add moving elements to grid
		SimObjectList::iterator_type it = allThings_->iterator();
		while(it != SimObjectList::NULL_NODE) {
			Thing* t = SimSchema::simObjectList.nextThing(it);
			if(t->isCollideable()) {
				LogMsg(t->name() << ": " << t->nextPos().worldCoor().toLog());
				collisionGrid_->insert(t->nextPos().worldCoor(), t->nextPos().radius(), *t);
			}
		}
	}


	void Area
	::setActive(bool state) {
		if(state == isActive_) return;
		isActive_ = state;

		if(!isActive_) moverCount_ = 0;

		SimObjectList::iterator_type it = allThings_->iterator();
		while(it != SimObjectList::NULL_NODE) {
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
		while(it != SimObjectList::NULL_NODE) {
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
		c->setScripts(*this, performer);
	}


	void Area
	::reset() {
		// Get newly spawned objects into allThings()
		flipChildren();

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
	::neighbour(const Coor& worldCoor) {
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


	inline bool _testActor2ThingCollision(Actor& actor1,
							  Thing& thing2) {
		// How close must the things be before colliding?
		// Double for pow 2 further down
		coor_t radSum = actor1.pos().radius() + thing2.pos().radius();
		coor_double_t radSumSq = radSum;
		radSumSq *= radSumSq;

		// If not colliding when taking the movement of both things
		// into account, then no collision
		if((actor1.nextPos().worldCoor().xzDistanceSquared(thing2.nextPos().worldCoor()) > radSumSq)
				|| (actor1.nextPos().worldCoor().yDistance(thing2.nextPos().worldCoor()) > radSum)
				) {
			return false;
		}

		// Inside collision range. Definitely collide.
		return actor1.pushThing(thing2);
	}

	void Area
	::testActors2ThingsCollisions() { // Actor** movers, short moverCount) {
		Assert(collisionGrid_);

		// Create buffer to temporarily hold collision candidates
		static const int MAX_THINGS = 256;
		// PS! Not thread safe, but takes less space on GBA stack
		static Thing* things[MAX_THINGS] VAR_IN_EWRAM;

		for(int outer = 0; outer < moverCount_; ++outer) {
			Actor* a = movers_[ outer ];

			short innerCount = collisionGrid_->collisionCandidates
				(a->nextPos().worldCoor(), a->nextPos().radius() + COOR_RES, things, MAX_THINGS - 1);

			// Test collision with all collision candidates
			for(int inner = 0; inner < innerCount; ++inner) {
				// Don't test collision with self
				if(things[ inner ] == a) {
					continue;
				}

				// Test for collision
				if(_testActor2ThingCollision(*a, *things[ inner ])) {
					a->resetFutureCoor();
					//LogMsg("Collision: " << a->name() << ", " << things[ inner ]->name());
					break;
				}
			}
		}
	}



	void Area
	::flipChildren(void) {
		static const int MAX_STACK_DEPTH = 10;
		SimObjectList::iterator_type itStack[ MAX_STACK_DEPTH ];

		Thing* t;

		int sp = 0;
		itStack[ 0 ] = childPosNodes().iterator();
		if(itStack[ 0 ] == SimObjectList::end())
			// No children at all..
			return;

		do {
			// Get next in chain
			PosNode* p = SimSchema::simObjectList.nextPosNode(itStack [ sp ]);
			Assert(p);

			// Move to new position in collision grid
			if(collisionGrid_
					&& p->isCollideable()
					&& p->pos().area() == p->nextPos().area()) {

				// TODO: Make CollisionGrid handle PosNodes
				Assert(p->isType(got_POS_NODE));
				t = static_cast<Thing*>(p);

				// TODO: Real speed instead of max speed...
				static const coor_t speed = COOR_RES;
				coor_t speedAndRadius = p->pos().radius() + speed;
				const Coor& wc = pos().world_.coor_;

				// TODO: Real speed instead of max speed...
				static const coor_t nextSpeed =  COOR_RES;
				coor_t nextSpeedAndRadius = p->nextPos().radius() + nextSpeed;
				const Coor& nextWC = nextPos().world_.coor_;

				collisionGrid_->move(wc, speedAndRadius, nextWC, nextSpeedAndRadius, *t);
			}

			// Do the flip
			p->flip();

			// Push child chain as next chain on stack
			itStack[ ++sp ] = p->childPosNodes().iterator();

			// Stack overflowed?
			Assert(sp < MAX_STACK_DEPTH);

			// Pop all completed chain
			while(sp >= 0 && itStack[ sp ] == SimObjectList::end()) {
				--sp;
			}
			// Continue if unpopped chains
		} while(sp >= 0);


		// Flip new spawns into area
		SimObjectList::iterator_type it = multiSimObjects_[ MGOA_SPAWNS ].iterator();
		while(it != SimObjectList::end()) {
			// Spawned things doesn't have to be actors
			Thing* t = SimSchema::simObjectList.nextThing(it);
			//LogMsg(t->pos().y_);

			// Newly spawned things will change area on first flip.
			// This will call Area::addThing, inserting it into
			// collisionGrid_ and other structures where it belong
			t->flip();
		}
		// New spawn are no longer new spawns once they are flipped
		multiSimObjects_[ MGOA_SPAWNS ].clear();
	}



	int Area
	::performChildPhysics(Actor** movers) {
		movers_ = movers;
		moverCount_ = 0;

		static const int MAX_STACK_DEPTH = 10;
		SimObjectList::iterator_type itStack[ MAX_STACK_DEPTH ];

		int sp = 0;
		itStack[ 0 ] = childPosNodes().iterator();
		if(itStack[ 0 ] == SimObjectList::NULL_NODE)
			// No children at all
			return 0;

		do {
			// Get next in chain
			Actor* a = SimSchema::simObjectList.nextActor(itStack [ sp ]);

			// Affect actors with the effects of the terrain
			// they are standing on.
			// PS! Placed here to avoid an extra loop. Cannot
			// be placed in move(), because flip may move
			// the actor into a new area or no area. Placing it
			// here should have no unwanted side effects.
			a->affect();

			// Reset values in nextPos that only last for a step
			//TODO: Must flick movement
			//a->nextMove().flick();

			// Calc next position
			if(a->calcNextCoor()) {
				// Add to movers
				movers[moverCount_++] = a;
			}


			// Push child chain as next chain on stack
			itStack[ ++sp ] = a->childPosNodes().iterator();

			// Stack overflowed?
			Assert(sp < MAX_STACK_DEPTH);

			// Pop all completed chain
			while(sp >= 0 && itStack[ sp ] == SimObjectList::end()) {
				--sp;
			}

			// Continue if there are still incomplete chains
		} while(sp >= 0);

		return moverCount_;
	}


	/*
	int Area
	::performPhysics(Actor** movers) {
		int moverCount = 0;

		// Get iterator for things with ability to move
		SimObjectList::iterator_type it = movingThings().iterator();
		while(it != SimObjectList::end()) {
			// Next actor with the ability to move
			Actor* a = SimSchema::simObjectList.nextActor(it);

			// Affect actors with the effects of the terrain
			// they are standing on.
			// PS! Placed here to avoid an extra loop. Cannot
			// be placed in move(), because flip may move
			// the actor into a new area or no area. Placing it
			// here should have no unwanted side effects.
			a->affect();

			// Reset values in nextPos that only last for a step
			//TODO: Must flick movement
			//a->nextMove().flick();

			// Calc next position
			if(a->calcNextCoor()) {
				// Add to movers
				movers[moverCount++] = a;
			}
			//movers[moverCount++] = a;
		}

		// Return number of movers
		return moverCount;
	}
	*/


	Thing* Area
	::spawn(const char* thingName, const ViewPoint& vp, long deniedTsMask, PosNode* parent) {
		if(deniedTsMask != 0 && (tsMask(terrainStyle(vp.coor_)) & deniedTsMask) != 0) {
			// Tried to spawn on denied terrain type
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
		thing->nextPos().setIndex( index(thing->nextPos().world_.coor_) );

		// Add the thing to the list of new spawns
		multiSimObjects_[ MGOA_SPAWNS ].add(*thing);

		// Return the newly created thing
		return thing;
	}


	long Area
	::touchedTerrain(const Coor& centre, coor_t radius) const {
		long touched = 0;
		Vector3 v(radius, 0, radius);
		for(coor_t dx = radius; dx > 0; dx -= COOR_RES) {
			for(coor_t dz = radius; dz > 0; dz -= COOR_RES) {
				Coor c;
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
	::touchedTerrain(const Coor& from, const Coor& to) const {
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
			touched |= tsMask(terrainStyle(Coor(CoorT::fromInt(x), 0, CoorT::fromInt(y))));
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
