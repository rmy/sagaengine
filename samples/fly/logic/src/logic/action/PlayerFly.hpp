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


#ifndef logic_PlayerFly_hpp
#define logic_PlayerFly_hpp

#include "sim/action/Action.hpp"
#include "util/vecmath/util_vecmath.hpp"
#include "util/vecmath/Euler3.hpp"

namespace logic {
	class _FlyLogicExport PlayerFly : public se_core::Action {
	public:
		PlayerFly() : se_core::Action("player_fly") {}

		short duration(se_core::ActionComponent& performer, se_core::Parameter& parameter) const { 
			return 1;
		}
		bool isContinuing(se_core::ActionComponent &performer, se_core::Parameter& parameter) const { 
			return false;
		}
		bool isRepeating(long when, se_core::ActionComponent &performer, se_core::Parameter& parameter) const {
			return true;
		}

		void perform(long when, se_core::ActionComponent& performer, se_core::Parameter& parameter) const;

		void param(float dirLR, float dirUD, float dirRoll, float speed, se_core::Parameter& out) const;

	private:
		struct Param {
			Param(se_core::Euler3& t, float speed)
				: torque_(t), speed_(speed) {
			}
			se_core::Euler3 torque_;
			float speed_;
		};
	};


	extern _FlyLogicExport const PlayerFly actionPlayerFly;
}

#endif
