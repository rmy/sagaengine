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

namespace se_core {
	namespace ClientSchema {
		extern _SeClientExport ClientListeners clientListeners;
		extern _SeClientExport ClientEventBridge clientEventBridge;
		extern _SeClientExport Phrase phrases;

		extern _SeClientExport Player* player;
		extern _SeClientExport Camera* floatingCamera;
		extern _SeClientExport Camera* camera;

	};


}


#endif

