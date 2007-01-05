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


#ifndef HealthListeners_hpp
#define HealthListeners_hpp

#include "../thing/Actor.hpp"
#include "../custom/StatComponent.hpp"

namespace se_core {
	class _SeCoreExport HealthListeners {
	public:
		HealthListeners();
		virtual ~HealthListeners();
		void add(StatComponent& actor, HealthListener& l);
		void add(HealthListener& l);
		void remove(HealthListener& l);
		void castHealthChangedEvent(StatComponent& actor, short change);

	private:
		static const int MAX_LISTENERS = 20;
		int listenerCount_;
		int* actorIds_;
		HealthListener** listeners_;
	};

}

#endif
