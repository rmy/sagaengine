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


#ifndef angel_script_ScriptFunctions_hpp
#define angel_script_ScriptFunctions_hpp

#include "sim/action/sim_action.hpp"
#include <string>

namespace se_core {
	class ScriptFunctions {
	public:
		static bool init();
		static void cleanup();

	public: // script commands
		static void log(const std::string& s);
		static void idle();
		static void idleMillis(int millis);
		static void startCutscene();

	public: // Helper methods to be used fro
		/** Suspend active script.
		 * This will suspend the active script until it is resumed.
		 * If the script is an AI script this will happen when the
		 * next action is returned.
		 */
		static void yield();

		static void resetAction();
		static void setAction(const Action& action, const Parameter* parameter = 0);
	};
}

#endif
