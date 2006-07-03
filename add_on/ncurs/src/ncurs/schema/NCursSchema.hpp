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


#ifndef ncurs_schema_NCursSchema_hpp
#define ncurs_schema_NCursSchema_hpp

#include "../gfx/canvas/ncurs_gfx_canvas.hpp"
#include "../gfx/frame/ncurs_gfx_frame.hpp"
#include "../area/ncurs_area.hpp"
#include "../input/ncurs_input.hpp"
#include "../thing/ThingEntityList.hpp"
#include "../thing/ncurs_thing.hpp"

namespace se_ncurs {
	namespace NCursSchema {
		extern Canvas& canvas();

		extern ThingEntityList::iterator_type firstThingEntity;
		extern ThingEntityList thingEntityList;
		extern WorldManager* worldManager;
		extern InputHandler* gameControls;
		extern Renderer* renderer;

		extern long gameClock;
	};
}

#endif
