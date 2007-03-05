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
#include "../SimComponent.hpp"
#include "../SimComposite.hpp"
#include "../AreaChildComponent.hpp"
#include "../custom/Move.hpp"
#include "../SimObject.hpp"
#include "../area/sim_area.hpp"
#include "../custom/sim_custom.hpp"
#include "../stat/sim_stat.hpp"
#include "../thing/sim_thing.hpp"
#include "../pos/PosComponent.hpp"

namespace se_core {
	class _SeCoreExport CollisionComponent : public AreaChildComponent {
	public:
		typedef Ptr<CollisionComponent, sct_COLLISION> Ptr;

		/** Constructor.
		 */
		CollisionComponent(Actor* owner, PosComponent* posComponent);
		CollisionComponent(Actor* owner);
		~CollisionComponent();

		static CollisionComponent* get(SimComposite& composite) {
			CollisionComponent* c = static_cast<CollisionComponent*>(composite.component(se_core::sct_COLLISION));
			return c;
		}

		static const CollisionComponent* get(const SimComposite& composite) {
			const CollisionComponent* c = static_cast<const CollisionComponent*>(composite.component(se_core::sct_COLLISION));
			return c;
		}

		static CollisionComponent* get(SimComponent& component) {
			CollisionComponent* c = static_cast<CollisionComponent*>(component.owner()->component(se_core::sct_COLLISION));
			return c;
		}

		int tag() const;
		bool shouldIgnore(const CollisionComponent& cc) const;
		void resetIgnore() {
			ignore_ = 0;
		}
		void setIgnore(const CollisionComponent& cc) {
			ignore_ = &cc;
		}


		/** Can other PosNodes collide with this PosNode?
		 */
		bool isCollideable() const {
			return isCollideable_;
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

		inline bool pushThing(CollisionComponent& pushedThing) {
			if(!collide_)
				return false;

			if(!pushedThing.isCollideable())
				return false;

			return collide_->collide(*this, pushedThing);
		}

		bool isPusher() const {
			return collide_ != 0;
		}

		bool didGeometryCollide(const CollisionComponent& other) const;
		bool doesGeometryCollide(const CollisionComponent& other) const;
		scale_t whenDoesGeometryCollide(const CollisionComponent& other) const;

		void areaChanged(SimComposite* newArea, SimComposite* oldArea);

		const BoundingBox& areaCovered() const { return areaCovered_; }
		void updateAreaCovered();
		void move();

		enum GeomType { geom_CYLINDER, geom_BOX };

		enum GeomType geometryType() const {
			if(posComponent_->nextPos().bounds_.minX_ == posComponent_->nextPos().bounds_.minZ_)
				return geom_CYLINDER;
			return geom_BOX;
		}

		coor_t bouncePoint(scale_t alpha, const Point3& testPoint, Point3& dest) const;
		coor_t bouncePoints(scale_t alpha, const CollisionComponent& other, Point3& d1, Point3& d2) const;


	private:
		PosComponent* posComponent_;
		BoundingBox areaCovered_;

		/** Flag that indicated wether other CollisionComponents can collide with this one.
		 * Used by the CollisionAreaComponent class to decide wether the CollisionComponent should be
		 * inserted into a collision grid.
		 */
		bool isCollideable_;
		const ThingCollide* collide_;

		const CollisionComponent* ignore_;
		enum GeomType geometryType_;
	};
}

#endif
