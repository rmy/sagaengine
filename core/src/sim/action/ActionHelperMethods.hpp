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


#ifndef ActionHelperMethods_hpp
#define ActionHelperMethods_hpp

#include "../thing/sim_thing.hpp"
#include "../area/sim_area.hpp"
#include "../stat/sim_stat.hpp"


namespace se_core {
	class ActionHelperMethods {
	public:
		/*
		static void calcWhenLimit(long when, Thing& performer);
		static bool isBlocking(Area* area, float x, float y);
		static bool isBlockingFront(Area* area, Pos& p);
		*/

		static void inform(char* text);
		static void say(Actor& speaker, char* text);

	};

}

#endif
