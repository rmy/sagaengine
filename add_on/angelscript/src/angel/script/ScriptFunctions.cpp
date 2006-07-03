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
#include <angelscript.h>


namespace se_core {

	bool ScriptFunctions
	::init() {
		// Configure the script engine with all the functions,
		// and variables that the script should be able to use.
		int r;

		// Register yield function
		r = AngelSchema::scriptEngine->RegisterGlobalFunction("void Idle()"
				, asFUNCTION(ScriptFunctions::idle), asCALL_CDECL);
		Assert( r >= 0 && "Idle");

		r = AngelSchema::scriptEngine->RegisterGlobalFunction("void Idle(int id)"
				, asFUNCTION(ScriptFunctions::idleMillis), asCALL_CDECL);
		Assert(r >= 0 && "Idle(millis)");

		r = AngelSchema::scriptEngine->RegisterGlobalFunction("void StartCutscene()"
				, asFUNCTION(ScriptFunctions::startCutscene), asCALL_CDECL);
		Assert( r >= 0 && "StartCutscene");

		r = AngelSchema::scriptEngine->RegisterGlobalFunction("void WasHere(int id)"
				, asFUNCTION(ScriptFunctions::wasHere), asCALL_CDECL);
		Assert( r >= 0 && "WasHere");

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
	::wasHere(int id) {
		LogMsg("WasHere: " << id);
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
		//LogMsg("WasHere: " << id);
	}

}
