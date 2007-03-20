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


#include <angelscript.h>
#include "scriptstring.h"
#include "AngelSchema.hpp"
#include "../io/AngelScriptParserModule.hpp"
#include "../script/ScriptFunctions.hpp"
#include "../script/Performer.hpp"
#include "io/schema/IoSchema.hpp"
#include "sim/action/ActionAndParameter.hpp"
#include "sim/InitListener.hpp"
#include "sim/InitListeners.hpp"
#include "sim/schema/SimSchema.hpp"
#include "util/error/Log.hpp"




namespace se_core {
	void AngelMessageCallback(const asSMessageInfo *msg, void *param) {
		const char *type = "ERR ";
		if( msg->type == asMSGTYPE_WARNING ) 
			type = "WARN";
		else if( msg->type == asMSGTYPE_INFORMATION ) 
			type = "INFO";

		LogDetail(msg->section << " (" << msg->row << ", " << msg->col << ") " << type << " " << msg->message);
	}

	/*
	class AngelOutputStream : public asIOutputStream {
	public:
		void Write(const char *text) {
			LogDetail("Angelscript message:\n" << text << "--------");
		}
	};
	*/


	namespace AngelSchema {
		asIScriptEngine *scriptEngine;

		ActionAndParameter& nextAction() {
			static ActionAndParameter ap;
			return ap;
		}

		const struct _SeAngelscriptExport AutoInit : public se_core::InitListener {

			AutoInit() {
				// Client event bridge should listen to init events.
				SimSchema::initListeners().addListener(*this);
				LogDetail("Registered Angelscript add-on");
			}


			~AutoInit() {
				SimSchema::initListeners().removeListener(*this);
				LogDetail("Cleaned up Angelscript add-on");
			}


			bool initEngineEvent() {
				// Create and register file parser modules
				static AngelScriptParserModule aspm(IoSchema::parser());

				LogDetail("Initializing angelscript version " << ANGELSCRIPT_VERSION_STRING);

				// Create the script engine
				scriptEngine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
				if( scriptEngine == 0 )
				{
					LogFatal("Failed to create script engine.");
					return false;
				}
				LogDetail("Angelscript engine created.");

				// The script compiler will send any compiler messages to
				// the outstream
				// The script compiler will send any compiler messages to the callback function
				scriptEngine->SetMessageCallback(asFUNCTION(AngelMessageCallback), 0, asCALL_CDECL);

				RegisterScriptString_Generic(scriptEngine);



				//static AngelOutputStream out;
				//scriptEngine->SetCommonMessageStream(&out);
				LogDetail("Angelscript log initialised.");

				// Register the script string type
				// Look at the implementation for this function for more information
				// on how to register a custom string type, and other object types.
				// The implementation is in "/add_on/scriptstring/scriptstring.cpp"
				//RegisterScriptString(scriptEngine);

				if(!Performer::init())
					return false;
				LogDetail("angelscript Perfomer object initialised");

				if(!ScriptFunctions::init())
					return false;
				LogDetail("angelscript functions initalised");

				LogDetail("Registered Angelscript add-on");
				return true;
			}


			void cleanupEngineEvent() {
				Performer::cleanup();
				ScriptFunctions::cleanup();
				scriptEngine->Release();

				LogDetail("Cleaned up Angelscript add-on");
			}

			bool initGameEvent() {
				return true;
			}


			void cleanupGameEvent() {
			}


			bool initLevelEvent() {
				return true;
			}

			void cleanupLevelEvent() {
			}

			bool startGameEvent() {
				return true;
			}

			void stopGameEvent() {
			}
		} autoInit;

	}

}

