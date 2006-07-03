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


#ifndef Health_hpp
#define Health_hpp

#include "util/config/util_config.hpp"

#ifndef SE_OWN_HEALTH

#include "../thing/sim_thing.hpp"

namespace se_core {
	class Health {
	public:
		Health();
		short maxHitpoints() const;
		short currentHitpoints(long when) const;
		void adjustHitpoints(Actor& actor, long when, int amount);

	private:
		long when_;
		short maxHitpoints_, currentHitpoints_;
		short hitpointRecoveryRate_;
		//int maxMana_, currentMana_;
		//int manaRecoveryRate_;
	};

}

#endif // SE_OWN_HEALTH

#endif
