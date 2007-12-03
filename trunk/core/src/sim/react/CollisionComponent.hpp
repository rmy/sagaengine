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


#ifndef engine_physics_CollisionComponent_hpp
#define engine_physics_CollisionComponent_hpp

#include "sim_react.hpp"
#include "ThingCollide.hpp"
#include "comp/Component.hpp"
#include "comp/Composite.hpp"
#include "comp/node/AreaChildComponent.hpp"
#include "../physics/Move.hpp"
#include "../area/sim_area.hpp"
#include "../custom/sim_custom.hpp"
#include "../stat/sim_stat.hpp"
#include "../thing/sim_thing.hpp"
#include "../pos/PosComponent.hpp"
#include "ContactInfo.hpp"

namespace se_core {
	class _SeCoreExport CollisionComponent : public AreaChildComponent {
	public:
		typedef ComponentPtr<CollisionComponent, sct_COLLISION> Ptr;

		/** Constructor.
		 */
		CollisionComponent(Composite* owner, const ComponentFactory* factory);
		~CollisionComponent();

		int tag() const;
		int substance() const {
			return substance_;
		}
		void setSubstance(int s) {
			substance_ = s;
		}
		void setDoObstructView(bool f) {
			doObstructView_ = f;
		}
		bool shouldIgnore(const CollisionComponent& cc) const;
		void resetIgnore() {
			ignore_ = 0;
		}
		void setIgnore(const CollisionComponent& cc) {
			ignore_ = cc.owner()->id();
		}


		/** Can other PosNodes collide with this PosNode?
		 */
		bool isCollideable() const {
			return isCollideable_ && !owner()->isDead();
		}

		/** Set wether this thing can be collided with.
		 * 
		 * This is a hint to the engine that this should be included
		 * in the collison space.
		 */
		void setCollideable(bool isCollideable);


		PosComponent& posComponent() {
			return *posComponent_;
		}

		const PosComponent& posComponent() const {
			return *posComponent_;
		}

		void setCollide(const ThingCollide* collide) {
			collide_ = collide; 
		}

		inline bool hasCollide() {
			return collide_ != 0;
		}


		inline bool pushThing(ContactInfo& pusher, ContactInfo& target) {
			if(!collide_)
				return false;

			return collide_->collide(pusher, target);
		}


		bool isPusher() const {
			return collide_ != 0;
		}

		bool didGeometryCollide(const CollisionComponent& other) const;
		bool doesGeometryCollide(const CollisionComponent& other) const;
		scale_t whenDoesGeometryCollide(const CollisionComponent& other) const;

		void zoneChanged(int zoneType, Composite* newArea, Composite* oldArea);

		const BoundingBox& areaCovered() const { return areaCovered_; }
		void updateAreaCovered();
		void move();

		enum GeomType { geom_CYLINDER, geom_LONG_CYLINDER };

		enum GeomType geometryType() const {
			BoundingBox& b = posComponent_->nextPos().bounds_;
			if(b.minX_ == b.minZ_ && b.maxX_ == b.maxZ_)
				return geom_CYLINDER;
			return geom_LONG_CYLINDER;
		}

		coor_t bouncePoint(const Point3& c, const Point3& testPoint, Point3& dest) const;
		coor_t bouncePoint(scale_t alpha, const Point3& testPoint, Point3& dest) const;
		coor_t bouncePoints(scale_t alpha, const CollisionComponent& other, Point3& d1, Point3& d2) const;
		coor_t bouncePoints(const Point3& o1, const Point3& o2, Point3& d1, Point3& d2) const;

		coor_t penetration(const CollisionComponent& other) const;

		bool doObstructView() const;

	private:
		PosComponent* posComponent_;
		BoundingBox areaCovered_;
		int substance_;

		/** Flag that indicated wether other CollisionComponents can collide with this one.
		 * Used by the CollisionAreaComponent class to decide wether the CollisionComponent should be
		 * inserted into a collision grid.
		 */
		bool isCollideable_, doObstructView_;
		const ThingCollide* collide_;

		Composite::id_type ignore_;

		enum GeomType geometryType_;
		Point3 p1_, p2_;
		coor_t radius_;
	};
}

#endif
