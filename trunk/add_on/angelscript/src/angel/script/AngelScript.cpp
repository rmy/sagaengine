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


#include "AngelScript.hpp"
#include "Performer.hpp"
#include "ScriptFunctions.hpp"
#include "../schema/AngelSchema.hpp"
#include "sim/thing/Actor.hpp"
#include "sim/action/ActionAndParameter.hpp"
#include "sim/action/Idle.hpp"
#include <angelscript.h>

namespace se_core {

	class AngelData : public ScriptData {
	public:
		AngelData(const Actor& actor) : mainCtx(0), eventCtx(0), performer(actor) {}
		~AngelData();

		// angelscript thread context
		asIScriptContext* mainCtx;
		asIScriptContext* eventCtx;

		//
		Performer performer;
	};


	AngelData
	::~AngelData() {
		// Release the context that is destroyed
		mainCtx->Release();
		eventCtx->Release();
		mainCtx = eventCtx = 0;
	}


	AngelScript
	::AngelScript(String* n) : Script(n) {
		initFuncId_ = AngelSchema::scriptEngine->GetFunctionIDByName(name(), "init");
		reinitFuncId_ = AngelSchema::scriptEngine->GetFunctionIDByName(name(), "reinit");
		sequenceFuncId_ = AngelSchema::scriptEngine->GetFunctionIDByName(name(), "sequence");
		transitionFuncId_ = AngelSchema::scriptEngine->GetFunctionIDByName(name(), "transition");
	}


	void AngelScript
	::reinit(const Actor& performer, ScriptData* sd) const {
		AngelData& data = static_cast<AngelData&>(*sd);
		if( reinitFuncId_ < 0 ) {
			LogMsg("The reinit() function was not found: " << name());
			return;
		}
		LogMsg(name() << ": " << performer.name());
		Assert(data.eventCtx);
		int r;
		r = data.eventCtx->Prepare(reinitFuncId_);
		Assert(r >= 0);
		data.eventCtx->SetArgObject(0, &data.performer);
		r = data.eventCtx->Execute();
		//Assert( r == asEXECUTION_SUSPENDED );
		WasHere();
	}


	ScriptData* AngelScript
	::init(const Actor& performer) const {
		LogMsg("Init: " << name() << ": " << performer.name());
		AngelData* data = new AngelData(performer);
		int r;

		//
		data->eventCtx = AngelSchema::scriptEngine->CreateContext();
		if( data->eventCtx == 0 ) {
			LogFatal("Failed to create context: " << name());
			return data;
		}
		if(reinitFuncId_ >= 0) {
		}

		// Create the angelscript thread context
		data->mainCtx = AngelSchema::scriptEngine->CreateContext();
		if( data->mainCtx == 0 ) {
			LogFatal("Failed to create context: " << name());
			return data;
		}

		// Find the function id for the function we want to execute.
		//int funcId = AngelSchema::scriptEngine->GetFunctionIDByName(name(), "sequence");
		if( sequenceFuncId_ < 0 ) {
			LogFatal("The sequence() function was not found: " << name());
			data->mainCtx->Release();
			data->mainCtx = 0;
			return data;
		}

		// Prepare it to execute the function
		r = data->mainCtx->Prepare(sequenceFuncId_);
		if( r < 0 ) {
			LogFatal("Failed to prepare the context: " << name());
			data->mainCtx = 0;
			return data;
		}

		data->mainCtx->SetArgObject(0, &data->performer);
		Assert(data->eventCtx);

		return data;
	}


	const Action* AngelScript
	::nextAction(const Actor& performer, int channel, ScriptData* sd, Parameter& out) const {
		Assert(sd);
		AngelData& data = static_cast<AngelData&>(*sd);
		Assert(data.mainCtx);
		if(!data.mainCtx)
			return 0;

		// No simultaneous actions for reentrant scripts. Always
		// put actions i action queue channel 0.
		if(channel != 0)
			return 0;

		if(transitionFuncId_ >= 0) {
			Assert(data.eventCtx);
			int r;
			r = data.eventCtx->Prepare(transitionFuncId_);
			Assert(r >= 0);
			data.eventCtx->SetArgObject(0, &data.performer);
			r = data.eventCtx->Execute();
			if( r == asEXECUTION_SUSPENDED ) {
			}
		}


		// No action chosen is default
		ScriptFunctions::resetAction();

		//LogMsg("Actor id: " << performer.id() << "  Channel: " << channel);
		// Execute or resume script
		int r = data.mainCtx->Execute();

		// Normally suspend script where it performs an action
		// (until that action is completed by the AI engine)
		if( r != asEXECUTION_SUSPENDED ) {
			// If reexecuted, refresh arg.
			data.mainCtx->SetArgObject(0, &data.performer);
			// The context has terminated execution (for one reason or other)
			// Tell actor that this script is dead
			return 0;
		}

		// If any action is chosen by the script, return it
		if(AngelSchema::nextAction().hasAction()) {
			out = AngelSchema::nextAction().parameter();
			return AngelSchema::nextAction().action();
		}

		// Keep one action thread running...
		return &actionIdle;
	}

}
