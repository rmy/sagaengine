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


#include "Abilities.hpp"

#ifndef SE_OWN_ABILITIES

namespace se_core {
	const short Abilities
	::autoBases_[][ABILITY_COUNT] = {
		{  5,  5,  5, 0 },
		{ 10, 10, 10, 1 },
		{ 15, 15, 15, 2 },
		{ 20, 20, 20, 3 },
		{ 25, 25, 25, 4 },
		{ 30, 30, 30, 5 }
	};


	Abilities
	::Abilities() {
		for(int i = 0; i < ABILITY_COUNT; ++i) {
			modifiers_[i] = 0;
		}
		setLevel(0);
	}


	Abilities
	::~Abilities() {
	}


	void Abilities
	::setLevel(short level) {
		for(int i = 0; i < ABILITY_COUNT; ++i) {
			bases_[ i ] = autoBases_[ level ][ i ];
		}
	}


	void Abilities
	::setBases(short* bases) {
		for(int i = 0; i < ABILITY_COUNT; ++i) {
			bases_[ i ] = bases[ i ];
		}
	}


	void Abilities
	::setBases(Abilities* src) {
		if(src) {
			for(int i = 0; i < ABILITY_COUNT; ++i) {
				bases_[ i ] = src->bases_[ i ];
			}
		}
		else {
			for(int i = 0; i < ABILITY_COUNT; ++i) {
				bases_[ i ] = 1;
			}
		}
	}


}

#endif // SE_OWN_ABILITIES

