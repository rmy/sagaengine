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
		/** Constructor.
		 */
		CollisionComponent(Actor* owner, PosComponent* posComponent);
		CollisionComponent(Actor* owner);
		~CollisionComponent();

		static CollisionComponent* get(SimComposite& composite) {
			CollisionComponent* c = static_cast<CollisionComponent*>(composite.component(se_core::sct_COLLISION));
			return c;
		}

		static CollisionComponent* get(SimComponent& component) {
			CollisionComponent* c = static_cast<CollisionComponent*>(component.owner()->component(se_core::sct_COLLISION));
			return c;
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

		void areaChanged(SimComposite* newArea, SimComposite* oldArea);

		const BoundingBox& areaCovered() const { return areaCovered_; }
		void updateAreaCovered();
		void move();


	private:
		PosComponent* posComponent_;
		BoundingBox areaCovered_;

		/** Flag that indicated wether other CollisionComponents can collide with this one.
		 * Used by the CollisionAreaComponent class to decide wether the CollisionComponent should be
		 * inserted into a collision grid.
		 */
		bool isCollideable_;
	};
}

#endif
