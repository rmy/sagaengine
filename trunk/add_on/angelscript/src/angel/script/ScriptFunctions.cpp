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


#include "ScriptFunctions.hpp"
#include "../schema/AngelSchema.hpp"
#include "util/error/Log.hpp"
#include "sim/action/ActionAndParameter.hpp"
#include "sim/action/all.hpp"
#include "basic/action/all.hpp"
#include "logic/action/all.hpp"
#include <angelscript.h>

using namespace se_basic;
using namespace logic;

namespace se_core {

	bool ScriptFunctions
	::init() {
		// Configure the script engine with all the functions,
		// and variables that the script should be able to use.
		int r;

		// Register yield function
		r = AngelSchema::scriptEngine->RegisterGlobalFunction("void idle()"
				, asFUNCTION(ScriptFunctions::idle), asCALL_CDECL);
		Assert( r >= 0 && "idle");

		r = AngelSchema::scriptEngine->RegisterGlobalFunction("void idle(int id)"
				, asFUNCTION(ScriptFunctions::idleMillis), asCALL_CDECL);
		Assert(r >= 0 && "idle(millis)");

		r = AngelSchema::scriptEngine->RegisterGlobalFunction("int random(int max)"
				, asFUNCTION(ScriptFunctions::random), asCALL_CDECL);
		Assert(r >= 0 && "int random(max)");

		r = AngelSchema::scriptEngine->RegisterGlobalFunction("void startCutscene()"
				, asFUNCTION(ScriptFunctions::startCutscene), asCALL_CDECL);
		Assert( r >= 0 && "startCutscene");

		r = AngelSchema::scriptEngine->RegisterGlobalFunction("void log(string& s)"
				, asFUNCTION(ScriptFunctions::log), asCALL_CDECL);
		Assert( r >= 0 && "log");

		return true;
	}


	void ScriptFunctions
	::cleanup() {
	}


	void ScriptFunctions
	::idle() {
		setAction(actionIdle);
		yield();
	}


	void ScriptFunctions
	::idleMillis(int millis) {
		static Parameter param;
		actionIdleFor.param(millis, param);
		ScriptFunctions::setAction(actionIdleFor, &param);
		ScriptFunctions::yield();
	}


	void ScriptFunctions
	::startCutscene() {
		setAction(actionStartCutscene);
		yield();
	}


	int ScriptFunctions
	::random(int max) {
		static int wait = 0;
		wait += TIMESTEP_INTERVAL + 1596;
		return wait % max;
	}

	void ScriptFunctions
	::yield() {
		asIScriptContext *ctx = asGetActiveContext();
		if( ctx )
		{
			// Suspend this context
			ctx->Suspend();
		}
	}


	void ScriptFunctions
	::log(const std::string& s) {
		LogMessage("AngelScript: " << s.c_str());
	}

	void ScriptFunctions
	::resetAction() {
		AngelSchema::nextAction().resetAction();
	}

	void ScriptFunctions
	::setAction(const Action& action, const Parameter* parameter) {
		AngelSchema::nextAction().setAction(action);
		if(parameter) {
			AngelSchema::nextAction().copyParameter(*parameter);
		}
		//LogMessage("WasHere: " << id);
	}

}
