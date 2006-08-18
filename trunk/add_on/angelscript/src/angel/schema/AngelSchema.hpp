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


#ifndef angel_script_AngelSchema_hpp
#define angel_script_AngelSchema_hpp

#include "sim/action/sim_action.hpp"
#include "util/system/util_system.hpp"
#include <angelscript.h>


namespace se_core {
	namespace AngelSchema {
		//
		extern asIScriptEngine *scriptEngine;

		// Next action to perform (should be set by angelscript function)
		extern ActionAndParameter& nextAction();

		//
		bool SE_AUTO_INIT init();
		void SE_AUTO_CLEANUP cleanup();
	}
}



#endif