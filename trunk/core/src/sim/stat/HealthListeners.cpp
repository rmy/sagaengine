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


#include "HealthListeners.hpp"
#include "HealthListener.hpp"
#include "../thing/Actor.hpp"

namespace se_core {
	HealthListeners
	::HealthListeners()
			: listenerCount_(0), actorIds_(new Composite::id_type[ MAX_LISTENERS ]), listeners_(new HealthListener*[ MAX_LISTENERS ]) {
		for(int i = 0; i < MAX_LISTENERS; ++i) {
			actorIds_[ i ] = 0;
			listeners_[ i ] = 0;
		}
	}

	HealthListeners
	::~HealthListeners() {
		delete[] actorIds_;
		delete[] listeners_;
	}


	void HealthListeners
	::add(StatComponent& actor, HealthListener& l) {
		listeners_[ listenerCount_ ] = &l;
		actorIds_[ listenerCount_ ] = actor.owner()->id();
		++listenerCount_;
	}


	void HealthListeners
	::add(HealthListener& l) {
		listeners_[ listenerCount_ ] = &l;
		actorIds_[ listenerCount_ ] = 0;
		++listenerCount_;
	}


	void HealthListeners
	::remove(HealthListener& l) {
		for(int i = 0; i < listenerCount_; ++i) {
			if(listeners_[ i ] == &l) {
				--listenerCount_;
				listeners_[ i ] = listeners_[ listenerCount_ ];
				break;
			}
		}
	}


	void HealthListeners
	::castHealthChangedEvent(StatComponent& actor, short change) {
		for(int i = 0; i < listenerCount_; ++i) {
			if(actorIds_[ i ] != 0 || actor.owner()->id() == actorIds_[ i ]) {
				listeners_[ i ]->healthChangedEvent(actor, change);
			}
		}
	}

}
