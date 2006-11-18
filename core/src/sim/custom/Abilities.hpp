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


#ifndef Abilities_hpp
#define Abilities_hpp

#include "util/config/util_config.hpp"

#ifndef SE_OWN_ABILITIES

namespace se_core {
	class _SeCoreExport Abilities {
	public:
		enum AbilityType { ATTACK, DEFENSE, SPEED, XP_LEVEL, ABILITY_COUNT };

		Abilities();
		virtual ~Abilities();
		void setLevel(short level);
		short modified(enum AbilityType type) const { return bases_[ type ] + modifiers_[ type ]; }
		void addModifier(enum AbilityType type, short modifier) { modifiers_[ type ] += modifier; }
		void subtractModifier(enum AbilityType type, short modifier) { modifiers_[ type ] -= modifier; }

		void addModifier(Abilities& abilities) {
			for(int i = 0; i < ABILITY_COUNT; ++i)
				modifiers_[ i ] += abilities.bases_[ i ] + abilities.modifiers_[ i ];
		}
		void subtractModifier(Abilities& abilities) {
			for(int i = 0; i < ABILITY_COUNT; ++i)
				modifiers_[ i ] -= abilities.bases_[ i ] + abilities.modifiers_[ i ];
		}
		void setBases(short* bases);
		void setBases(Abilities* src);

	private:
		short bases_[ ABILITY_COUNT ];
		short modifiers_[ ABILITY_COUNT ];
		const static short autoBases_[][ABILITY_COUNT];
	};
#endif // SE_OWN_ABILITIES

}

#endif
