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


#ifndef engine_physics_CollisionAreaComponent_hpp
#define engine_physics_CollisionAreaComponent_hpp

#include "comp/node/NodeComponent.hpp"
#include "CollisionGrid.hpp"
#include "CollisionComponent.hpp"
#include "ContactInfo.hpp"
#include "AreaEdge.hpp"
#include "../area/Area.hpp"

namespace se_core {
	class _SeCoreExport Contact {
	public:
		ContactInfo ci1_;
		ContactInfo ci2_;
		coor_t radSum_;

		bool doesGeometryCollide() const {
			bool res = (ci1_.bouncePoint_.xzDistanceSquared(ci1_.bouncePoint_) < radSum_ * radSum_);
			return res;
		}
	};


	class _SeCoreExport CollisionAreaComponent : public NodeComponent {
	public:
		typedef ComponentPtr<CollisionAreaComponent, sct_COLLISION_AREA> Ptr;

		/** Constructor.
		 */
		CollisionAreaComponent(Composite* owner);
		~CollisionAreaComponent();

		const char* name() const { return "CollisionArea"; }

		CollisionGrid* collisionGrid() {
			return collisionGrid_; 
		}

		AreaEdge& areaEdge() {
			return *areaEdge_;
		}
		const AreaEdge& areaEdge() const {
			return *areaEdge_;
		}

		void setActive(bool state);

		void addCollideable(CollisionComponent& cc);
		void removeCollideable(CollisionComponent& cc);

		void getContactList(Contact* list, short& count, int maxCollisions);
		coor_t farthestLineOfSight(const Point3& fromPoint, const Point3& toPoint) const;
		short _collisionCandidates(CollisionComponent *cc, const int MAX_THINGS, CollisionComponent *candidates[]);
		short _collisionCandidates2(CollisionComponent *cc, const int MAX_THINGS, CollisionComponent *candidates[]);
		void _testCollisionCandidates(CollisionComponent *cc, short candidateCount, CollisionComponent *candidates[], int maxCollisions, Contact *list, short& count);
		void _testCollisionCandidates2(CollisionComponent *cc, short candidateCount, CollisionComponent *candidates[], int maxCollisions, Contact *list, short& count);

	private:
		/**
		 * Sets the grid used to speed up the collisions detection.
		 * Often there an area is inactive much of the time, and a fine grained grid
		 * may take up quite a lot of memory. So the grid is passed around from
		 * one active area to the next.
		 */
		void setCollisionGrid(CollisionGrid* grid);
		void resetCollisionGrid();

		CollisionGrid* collisionGrid_;
		AreaEdge* areaEdge_;
	};

}


#endif

