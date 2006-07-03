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


#ifndef game_action_ShootAcid_hpp
#define game_action_ShootAcid_hpp

#include "sim/action/Action.hpp"


namespace game {
	class ShootAcid : public se_core::Action {
	public:
		ShootAcid() : se_core::Action("laser_attack") {}

		short duration(se_core::Actor& performer, se_core::Parameter& parameter) const;
		bool isContinuing(se_core::Actor &performer, se_core::Parameter& parameter) const;

		short channel() const { return se_core::CHANNEL_EXTRA; }

		bool isRepeating(long when, se_core::Actor &performer, se_core::Parameter& parameter) const {
			return false;
		}

		void perform(long when, se_core::Actor& performer, se_core::Parameter& parameter) const;

	private:
		void spawn(long when, se_core::Actor& performer) const;
	};

	extern const ShootAcid actionShootAcid;
}

#endif
