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


#include "Scripting.hpp"
#include "GamePre.H"

namespace game {
	/*
	void move() {
		ScriptFunctions::setAction(actionMove);
		ScriptFunctions::yield();
	}


	void turn8(int dir8) {
		ScriptFunctions::setAction(*actionTurnArray[ dir8 & 0x7 ]);
		ScriptFunctions::yield();
	}


	void shootAcid() {
		ScriptFunctions::setAction(actionLaserAttack);
		ScriptFunctions::yield();
	}
	*/


	bool Scripting
	::init() {
		/*
		int r;
		r = AngelSchema::scriptEngine->RegisterGlobalFunction("void Move()"
				, asFUNCTION(move), asCALL_CDECL);
		Assert(r >= 0 && "Move");

		r = AngelSchema::scriptEngine->RegisterGlobalFunction("void Turn8(int id)"
				, asFUNCTION(turn8), asCALL_CDECL);
		Assert(r >= 0 && "Turn8");

		r = AngelSchema::scriptEngine->RegisterGlobalFunction("void ShootAcid()"
				, asFUNCTION(shootAcid), asCALL_CDECL);
		Assert(r >= 0 && "ShootAcid");
		*/

		return true;
	}


	void Scripting
	::cleanup() {
	}
}
