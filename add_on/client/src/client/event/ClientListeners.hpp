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


#ifndef ClientListeners_hpp
#define ClientListeners_hpp

#include "client_event.hpp"
#include "sim/area/sim_area.hpp"
#include "sim/thing/sim_thing.hpp"

namespace se_core {
	class ClientListeners {
	public:
		ClientListeners() : listenerCount(0) {}
		void addListener(ClientListener& l);
		void removeListener(ClientListener& l);
		void castCameraEnteredAreaEvent(Area& area);
		void castCameraLeftAreaEvent(Area& area);
		void castThingEnteredCameraAreaEvent(Thing& thing);
		void castThingLeftCameraAreaEvent(Thing& thing);


		void castThingEnteredActiveZoneEvent(Thing& thing);
		void castThingLeftActiveZoneEvent(Thing& thing);
		void castThingSwitchedActiveAreaEvent(Thing& thing);

	private:
		static const int MAX_LISTENERS = 10;
		ClientListener* listeners[ MAX_LISTENERS ];
		short listenerCount;
	};

}


#endif
