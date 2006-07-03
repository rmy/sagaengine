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


#include "GamePre.H"
#include "InitAngelscript.hpp"
#include "../action/all.hpp"

using namespace se_core;

namespace game {
	void forward() {
		ScriptFunctions::setAction(actionForward);
		ScriptFunctions::yield();
	}


	void backward() {
		ScriptFunctions::setAction(actionBackward);
		ScriptFunctions::yield();
	}


	void stop() {
		ScriptFunctions::setAction(actionStop);
		ScriptFunctions::yield();
	}


	void seek() {
		ScriptFunctions::setAction(actionSeek);
		ScriptFunctions::yield();
	}


	void targetPlayer() {
		ScriptFunctions::setAction(actionTargetPlayer);
		ScriptFunctions::yield();
	}


	void shootAcid() {
		ScriptFunctions::setAction(actionShootAcid);
		ScriptFunctions::yield();
	}


	bool InitAngelscript
	::init() {
		int r;
		r = AngelSchema::scriptEngine->RegisterGlobalFunction("void Forward()"
				, asFUNCTION(forward), asCALL_CDECL);
		Assert(r >= 0 && "Forward");

		r = AngelSchema::scriptEngine->RegisterGlobalFunction("void Backward()"
				, asFUNCTION(backward), asCALL_CDECL);
		Assert(r >= 0 && "Backward");

		r = AngelSchema::scriptEngine->RegisterGlobalFunction("void Stop()"
				, asFUNCTION(stop), asCALL_CDECL);
		Assert(r >= 0 && "Stop");

		r = AngelSchema::scriptEngine->RegisterGlobalFunction("void Seek()"
				, asFUNCTION(seek), asCALL_CDECL);
		Assert(r >= 0 && "Seek");

		r = AngelSchema::scriptEngine->RegisterGlobalFunction("void TargetPlayer()"
				, asFUNCTION(targetPlayer), asCALL_CDECL);
		Assert(r >= 0 && "TargetPlayer");

		/*
		r = AngelSchema::scriptEngine->RegisterGlobalFunction("void Turn8(int id)"
				, asFUNCTION(turn8), asCALL_CDECL);
		Assert(r >= 0 && "Turn8");
		*/

		r = AngelSchema::scriptEngine->RegisterGlobalFunction("void ShootAcid()"
				, asFUNCTION(shootAcid), asCALL_CDECL);
		Assert(r >= 0 && "ShootAcid");

		return true;
	}

}
