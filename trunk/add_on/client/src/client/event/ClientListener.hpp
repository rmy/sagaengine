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


#ifndef ClientListener_hpp
#define ClientListener_hpp

#include "sim/area/sim_area.hpp"
#include "sim/thing/sim_thing.hpp"

namespace se_client {

	class _SeClientExport ClientListener {
	public:
		virtual ~ClientListener() {} // Gets rid of warning
		virtual void cameraEnteredAreaEvent(se_core::Area& area) = 0;
		virtual void cameraLeftAreaEvent(se_core::Area& area) = 0;
		/*
		virtual void thingEnteredCameraAreaEvent(se_core::Thing& thing) {}
		virtual void thingLeftCameraAreaEvent(se_core::Thing& thing) {}
		//
		virtual void thingEnteredActiveZoneEvent(se_core::Thing& thing) = 0;
		virtual void thingLeftActiveZoneEvent(se_core::Thing& thing) = 0;
		virtual void thingSwitchedActiveAreaEvent(se_core::Thing& thing) = 0;
		*/
	};

}

#endif
