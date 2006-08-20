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


#ifndef ClientSchema_hpp
#define ClientSchema_hpp

#include "../event/client_event.hpp"
#include "../lang/client_lang.hpp"
#include "sim/thing/sim_thing.hpp"
#include "util/system/util_system.hpp"

/**
 * The Client add-on module works as a bridge between SagaEngine Core and client application.
 * This add-on contains pointers the player and camera Actor objects, it contains language suppoert,
 * and an event bridge that is useful between the core and render modules.
 */
namespace se_client {
	/**
	 * Global objects that are useful for client applications.
	 * 
	 */
	namespace ClientSchema {
		/** Manager for client listeners. */
		extern _SeClientExport ClientListeners clientListeners;
		
		/** Bridge for events between the core and client application + set the present camera throught this class. */
		extern _SeClientExport ClientEventBridge clientEventBridge;
		
		/** Look up dictionary that contains phrases in several languages. */
		extern _SeClientExport Phrase phrases;

		
		/** The current player object */
		extern _SeClientExport se_core::Player* player;

		/** A camera object (actor) that can float free of the player object. */
		extern _SeClientExport se_core::Camera* floatingCamera;

		/** The object that the camera is attached to. Probably the player or the floatingCamera */
		extern _SeClientExport se_core::Camera* camera;

	};


}


#endif

