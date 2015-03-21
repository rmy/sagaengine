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

namespace se_client {
	class _SeClientExport ClientListeners {
	public:
		ClientListeners() : listenerCount(0), listeners(new ClientListener*[ MAX_LISTENERS ]) {}
		virtual ~ClientListeners() { delete[] listeners; }
		void addListener(ClientListener& l);
		void removeListener(ClientListener& l);
		void castCameraEnteredAreaEvent(se_core::Area& area);
		void castCameraLeftAreaEvent(se_core::Area& area);
		/*
		void castThingEnteredCameraAreaEvent(se_core::Thing& thing);
		void castThingLeftCameraAreaEvent(se_core::Thing& thing);


		void castThingEnteredActiveZoneEvent(se_core::Thing& thing);
		void castThingLeftActiveZoneEvent(se_core::Thing& thing);
		void castThingSwitchedActiveAreaEvent(se_core::Thing& thing);
		*/

	private:
		static const int MAX_LISTENERS = 10;
		ClientListener** listeners;
		short listenerCount;
	};

}


#endif
