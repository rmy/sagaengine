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


#ifndef StatComponent_hpp
#define StatComponent_hpp

#include "Abilities.hpp"
#include "Health.hpp"
#include "comp/Component.hpp"
#include "../sim.hpp"
#include "../action/Action.hpp"
#include "../action/ActionAndParameter.hpp"
#include "../config/sim_config.hpp"
#include "../script/sim_script.hpp"
#include "../stat/sim_stat.hpp"
#include "../pos/sim_pos.hpp"
#include "../thing/sim_thing.hpp"
#include "comp/Composite.hpp"
#include "comp/Component.hpp"
#include "util/type/util_type.hpp"
#include "sim/stat/PropertyHashTable.hpp"

namespace se_core {
	class _SeCoreExport StatComponent  : public Component {
	public:
		typedef ComponentPtr<StatComponent, sct_STAT> Ptr;

		static StatComponent* get(Composite& composite) {
			StatComponent* c = static_cast<StatComponent*>(composite.component(se_core::sct_STAT));
			return c;
		}

		static StatComponent* get(Component& component) {
			StatComponent* c = static_cast<StatComponent*>(component.owner()->component(se_core::sct_STAT));
			return c;
		}

		static const StatComponent* get(const Component& component) {
			const StatComponent* c = static_cast<const StatComponent*>(component.owner()->component(se_core::sct_STAT));
			return c;
		}

		StatComponent(Composite* owner, const ComponentFactory* factory = 0);
		//void setAbilities(short* abilities);

		coor_t walkSpeed() const;
		Abilities* abilities() { return &abilities_; }
		const Abilities* abilities() const { return &abilities_; }
		Health& health() { return health_; }
		const Health& health() const { return health_; }

		bool hasDefaultAction() const {
			return defaultAction_.hasAction();
		}
		const ActionAndParameter& defaultAction() const { 
			return defaultAction_; 
		}
		const Action* defaultAction(Parameter& out) const { 
			out = defaultAction_.parameter();
			return defaultAction_.action(); 
		}
		void setDefaultAction(const Action& action, const Parameter* parameter = 0) { 
			defaultAction_.setAction(action);
			if(parameter) {
				defaultAction_.copyParameter(*parameter);
			}
		}
		void resetDefaultAction() { defaultAction_.resetAction(); }


		const Composite* target() const { return target_.object(); }
		Composite* target() { return target_.object(); }
		PosComponent* targetPos();
		const PosComponent* targetPos() const;
		bool hasTarget() const { return !target_.isNull(); }
		void setTarget(Composite* target) { target_.set(target->ref()); }
		void resetTarget() { target_.reset(); }

		const Composite* item() const { return item_.object(); }
		Composite* item() { return item_.object(); }
		PosComponent* itemPos();
		const PosComponent* itemPos() const;
		bool hasItem() const { return !item_.isNull(); }
		void setItem(Composite* item) { item_.set(item->ref()); }
		void resetItem() { item_.reset(); }

		void setShouldSave(bool state) { shouldSave_ = state; }
		bool shouldSave() { return shouldSave_; }

		const Property* property(const char* name) const;
		const Property* property(int key) const;

	protected:
		static const int MGO_COUNT = 20;
		static const int SV_COUNT = 20;
		static const int ATT_COUNT = 20;

		mutable short singleValues_[SV_COUNT];
		String attributes_[ATT_COUNT];


	protected:
		friend class StatComponentFactory;
		bool shouldSave_;
		Abilities abilities_;
		Health health_;
		const PropertyHashTable* properties_;
		mutable ActionAndParameter defaultAction_;
		Composite::RefPtr target_;
		Composite::RefPtr item_;
	};

}

#endif
