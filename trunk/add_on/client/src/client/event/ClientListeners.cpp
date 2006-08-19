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


#include "ClientListeners.hpp"
#include "ClientListener.hpp"
#include "util/error/Log.hpp"

using namespace se_core;

namespace se_client {

	void ClientListeners
	::addListener(ClientListener& l) {
		Assert(listenerCount < MAX_LISTENERS && "Added one listener too many");
		listeners[ listenerCount++ ] = &l;
	}


	void ClientListeners
	::removeListener(ClientListener& l) {
		for(int i = 0; i < listenerCount; ++i) {
			if(listeners[ i ] == &l) {
				listeners[ i ] = listeners[ --listenerCount ];
			}
		}
	}


	void ClientListeners
	::castCameraEnteredAreaEvent(Area& area) {
		for(int i = 0; i < listenerCount; ++i) {
			listeners[ i ]->cameraEnteredAreaEvent(area);
		}
	}


	void ClientListeners
	::castCameraLeftAreaEvent(Area& area) {
		for(int i = 0; i < listenerCount; ++i) {
			listeners[ i ]->cameraLeftAreaEvent(area);
		}
	}


	void ClientListeners
	::castThingEnteredCameraAreaEvent(Thing& thing) {
		for(int i = 0; i < listenerCount; ++i) {
			listeners[ i ]->thingEnteredCameraAreaEvent(thing);
		}
	}


	void ClientListeners
	::castThingLeftCameraAreaEvent(Thing& thing) {
		for(int i = 0; i < listenerCount; ++i) {
			listeners[ i ]->thingLeftCameraAreaEvent(thing);
		}
	}


	void ClientListeners
	::castThingEnteredActiveZoneEvent(Thing& thing) {
		for(int i = 0; i < listenerCount; ++i) {
			listeners[ i ]->thingEnteredActiveZoneEvent(thing);
		}
	}


	void ClientListeners
	::castThingLeftActiveZoneEvent(Thing& thing) {
		for(int i = 0; i < listenerCount; ++i) {
			listeners[ i ]->thingLeftActiveZoneEvent(thing);
		}
	}


	void ClientListeners
	::castThingSwitchedActiveAreaEvent(Thing& thing) {
		for(int i = 0; i < listenerCount; ++i) {
			listeners[ i ]->thingSwitchedActiveAreaEvent(thing);
		}
	}


}
