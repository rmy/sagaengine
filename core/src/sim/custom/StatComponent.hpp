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
#include "../sim.hpp"
#include "../SimComponent.hpp"
#include "../action/Action.hpp"
#include "../config/sim_config.hpp"
#include "../script/sim_script.hpp"
#include "../stat/sim_stat.hpp"
#include "../thing/sim_thing.hpp"
#include "../SimComposite.hpp"
#include "../SimComponent.hpp"
#include "util/type/util_type.hpp"

namespace se_core {
	class _SeCoreExport StatComponent  : public SimComponent {
	public:
		typedef Ptr<StatComponent, sct_STAT> Ptr;

		static StatComponent* get(Composite& composite) {
			StatComponent* c = static_cast<StatComponent*>(composite.component(se_core::sct_STAT));
			return c;
		}

		static StatComponent* get(SimComponent& component) {
			StatComponent* c = static_cast<StatComponent*>(component.owner()->component(se_core::sct_STAT));
			return c;
		}

		static const StatComponent* get(const SimComponent& component) {
			const StatComponent* c = static_cast<const StatComponent*>(component.owner()->component(se_core::sct_STAT));
			return c;
		}

		StatComponent(Composite* owner, const SimComponentFactory* factory = 0);
		void setAbilities(short* abilities);
		void setQuickMenuAction(const Action* a);
		void setUseAction(const Action* a);

		coor_t walkSpeed() const;
		Abilities* abilities() { return &abilities_; }
		const Abilities* abilities() const { return &abilities_; }
		Health& health() { return health_; }
		const Health& health() const { return health_; }

		/*

		enum Special { NONE, BUBBLE_POWER, SPEED, SHOCK_WAVE };
		void setSpecial(enum Special special, long millis);
		enum Special special() const;
		bool isSpecialEndingSoon() const;
		*/

	protected:
		friend class StatComponentFactory;
		/*
		enum Special special_;
		long specialWhen_;
		*/
		Abilities abilities_;
		Health health_;
		const Action* quickMenuAction_;
		const Action* useAction_;
	};

}

#endif
