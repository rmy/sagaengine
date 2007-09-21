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

#include "CollisionAreaComponent.hpp"
#include "CollisionManager.hpp"
#include "CollisionComponent.hpp"
#include "../schema/SimSchema.hpp"
#include "util/error/Log.hpp"
#include "util/bounds/BoundingBox.hpp"
#include "../thing/Actor.hpp"
#include "../zone/ZoneAreaComponent.hpp"
#include "../pos/PosComponent.hpp"
#include <cstdlib>



namespace se_core {
	static coor_t MAX_SPEED = 64 * COOR_STEP + 2 * COOR_RES;

	CollisionAreaComponent
	::CollisionAreaComponent(Composite* owner) 
		: NodeComponent(sct_COLLISION, owner)
		, collisionGrid_(0), areaEdge_(new AreaEdge()) {
	}


	CollisionAreaComponent
	::~CollisionAreaComponent() {
		delete areaEdge_;
 	}


	void CollisionAreaComponent
	::addCollideable(CollisionComponent& cc) {
		if(collisionGrid_) {
			Point3 p;
			cc.areaCovered().center(p);
			coor_t speedAndRadius = cc.areaCovered().radius();

			PosComponent* pc = &cc.posComponent();
			collisionGrid_->insert(p, speedAndRadius, cc);
		}
 	}


	void CollisionAreaComponent
	::removeCollideable(CollisionComponent& cc) {
		if(!collisionGrid_)
			return;

		Point3 p;
		cc.areaCovered().center(p);
		coor_t speedAndRadius = cc.areaCovered().radius();
		bool didDelete = collisionGrid_->remove(p, speedAndRadius, cc);

		AssertFatal(didDelete, "Couldn't remove " << cc.owner()->name() << " from collision grid (" << collisionGrid_->find(cc) << ")");
	}


	void CollisionAreaComponent
	::setCollisionGrid(CollisionGrid* grid) {
		collisionGrid_ = grid;

		// Make sure the grid does not have any members
		// from previous usage
		collisionGrid_->clear();

		// Align the grid coordinate system with
		// this areas coordinate system
		const Area::Ptr a(this);
		const coor_tile_t w = a->width();
		const coor_tile_t h = a->height();

		// TODO: Improve this desperate solution for border creatures (* 2)
		// see aldo CollisionManager
		collisionGrid_->setSize(w * 2, h * 2);

		PosComponent::Ptr pos(*this);
		Point3 offset(pos->pos().worldCoor());
		offset.sub(Point3(CoorT::fromTile(w / 2), 0, CoorT::fromTile(h / 2)));

		collisionGrid_->setOffset(offset);

		// Add collideable elements to grid
		NodeComponentList::Iterator it(children_);
		while(it.hasNext()) {
			CollisionComponent* cc = static_cast<CollisionComponent*>(&it.next());
			if(!cc->isCollideable())
				continue;

			cc->updateAreaCovered();

			Point3 newPos;
			cc->areaCovered().center(newPos);
			coor_t newRadius = cc->areaCovered().radius();


			PosComponent* p = &cc->posComponent();
			//LogDetail(p->pos().worldCoor() << " - " << p->pos().localCoor());
			//LogDetail(p->nextPos().worldCoor() << " - " << p->nextPos().localCoor());
			//LogDetail(newPos << " - " << newRadius);
			collisionGrid_->insert(newPos, newRadius, *cc);
		}
	}


	void CollisionAreaComponent
	::resetCollisionGrid() {
		CollisionGrid* g = collisionGrid_;
		if(collisionGrid_) {
			CollisionManager::singleton().releaseCollisionGrid(collisionGrid_);
			collisionGrid_ = 0;
		}
	}


	void CollisionAreaComponent
	::setActive(bool state) {
		if(state) {
			setCollisionGrid(CollisionManager::singleton().grabCollisionGrid());
			setParent(CollisionManager::singleton());
		}
		else {
			resetParent();
			resetCollisionGrid();
		}
	}


	inline bool _testCollision(CollisionComponent& cc1,
							  CollisionComponent& cc2) {
		if(!cc1.areaCovered().isTouching(cc2.areaCovered()))
			return false;

		if(!cc1.doesGeometryCollide(cc2))
			return false;

		if(cc1.shouldIgnore(cc2))
			return false;

		// Inside collision range. Collide.
		return !cc1.isDead() && !cc2.isDead();
	}

	coor_t CollisionAreaComponent
	::farthestLineOfSight(const se_core::Point3& fromPoint, const se_core::Point3& toPoint) const {
		const int MAX_CONTACTS = 128;
		static CollisionComponent* candidates[MAX_CONTACTS];
		int candidateCount = 0;
		Contact contacts[ MAX_CONTACTS ];
		int contactCount = 0;
		coor_t dist = CoorT::sqrt(fromPoint.xzDistanceSquared(toPoint));
		coor_t speedAndRadius = CoorT::half(dist);

		Point3 middle;
		middle.add(fromPoint, toPoint);
		middle.scale(0.5f);

		BoundingBox bounds(middle, speedAndRadius);

		ZoneAreaComponent::Ptr aZone(*this);
		ComponentList::Iterator linkIt(aZone->links());
		while(linkIt.hasNext()) {
			ZoneAreaComponent& a = static_cast<ZoneAreaComponent&>(linkIt.next());
			CollisionAreaComponent::Ptr cac(a);
			if(!cac->collisionGrid())
				continue;

			candidateCount += cac->collisionGrid()->collisionCandidates
				(middle, speedAndRadius, &candidates[candidateCount], MAX_CONTACTS - candidateCount);
		}

		// Test collision with all collision candidates
		for(int c = 0; c < candidateCount; ++c) {
			// Test for collision
			CollisionComponent& cc = *candidates[ c ];

			if(!cc.doObstructView())
				continue;

			if(!bounds.isTouching(cc.areaCovered()))
				continue;

			Point3 losNearest, candidateNearest;
			coor_t candRadius = cc.bouncePoints(fromPoint, toPoint, candidateNearest, losNearest);
			if(candidateNearest.xzDistanceSquared(losNearest) < candRadius * candRadius
					&& fromPoint.xzDistanceSquared(losNearest) < (dist + candRadius) * (dist + candRadius) ) {
				dist = CoorT::sqrt(fromPoint.xzDistanceSquared(losNearest));
				dist -= candRadius;
			}
		}
		return dist;		
	}

	int CollisionAreaComponent
	::getContactList(Contact* list, int maxCollisions) {
		Point3 tmp;
		int count = 0;
		int outer = 0;

		if(!collisionGrid_)
			return count;

		static const int MAX_THINGS = 256;
		static CollisionComponent* candidates[MAX_THINGS];
		ZoneAreaComponent::Ptr aZone(*this);

		NodeComponentList::Iterator it(children_);
		while(it.hasNext()) {
			CollisionComponent* cc = static_cast<CollisionComponent*>(&it.next());
			++outer;

			short innerCount = 0;
			ComponentList::Iterator linkIt(aZone->links());
			while(linkIt.hasNext()) {
				ZoneAreaComponent& a = static_cast<ZoneAreaComponent&>(linkIt.next());
				CollisionAreaComponent::Ptr cac(a);
				if(!cac->collisionGrid())
					continue;

				Point3 p;
				cc->areaCovered().center(p);
				coor_t speedAndRadius = cc->areaCovered().radius();
				innerCount += cac->collisionGrid()->collisionCandidates
					(p, speedAndRadius, &candidates[innerCount], MAX_THINGS - innerCount);
			}


			// Test collision with all collision candidates
			for(int inner = 0; inner < innerCount; ++inner) {
				// Don't test collision with self
				if(candidates[ inner ] == cc) {
					continue;
				}

				// Test for collision
				CollisionComponent& cc2 = *candidates[ inner ];

				// Only collide once (and at least once)
				if((size_t)cc < (size_t)(&cc2) && cc->isCollideable() && cc2.isCollideable()) {
					continue;
				}

				if(_testCollision(*cc, cc2)) {
					// Only collide once (and at least once)
					bool didAlready = false;
					for(int i = 0; i < count; ++i) {
						if(list[ i ].ci1_.cc_ == &cc2 && list[ i ].ci2_.cc_ == cc) {
							didAlready = true;
							break;
						}
					}

					if(!didAlready) {
						Assert(count < maxCollisions);
						if(count > 64) {
							LogWarning(cc->owner()->name() << " - " << cc2.owner()->name());
						}
						Contact& c = list[ count ];
						c.ci1_.cc_ = cc;
						c.ci2_.cc_ = &cc2;
						c.ci1_.vp_.setViewPoint(cc->posComponent().nextPos().world_);
						c.ci2_.vp_.setViewPoint(cc2.posComponent().nextPos().world_);

						// Calculate radius and bounce points
						Point3& d1 = c.ci1_.bouncePoint_;
						Point3& d2 = c.ci2_.bouncePoint_;
						c.ci1_.radius_ = cc->bouncePoint(c.ci1_.vp_.coor_, c.ci2_.vp_.coor_, d1);
						c.ci2_.radius_ = cc2.bouncePoint(c.ci2_.vp_.coor_, d1, d2);
						d1.y_ = d2.y_;
						c.radSum_ = c.ci1_.radius_ + c.ci2_.radius_;

						++count;
					}
				}
			}
		}

		return count;
	}

}
