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


#include "Performer.hpp"
#include "../schema/AngelSchema.hpp"
#include "se_core.hpp"
#include <angelscript.h>

namespace se_core {

	Performer
	::Performer(const Actor& actor) : actor_(actor), refCount_(0) {
	}


	bool Performer
	::init() {
		int r;

		r = AngelSchema::scriptEngine->RegisterObjectType("Performer", 0, asOBJ_CLASS);
		Assert(r >= 0 && "Failed to create object");

		r = AngelSchema::scriptEngine->RegisterObjectBehaviour("Performer", asBEHAVE_ADDREF, "void f()", asMETHOD(Performer, addRef), asCALL_THISCALL);
		Assert(r >= 0 && "Failed to register addref");

		AngelSchema::scriptEngine->RegisterObjectBehaviour("Performer", asBEHAVE_RELEASE, "void f()", asMETHOD(Performer, release), asCALL_THISCALL);
		Assert(r >= 0 && "Failed to register release");

		r = AngelSchema::scriptEngine->RegisterObjectMethod("Performer", "bool didMove()", asMETHOD(Performer, didMove), asCALL_THISCALL);
		Assert(r >= 0 && "Failed to register method didMove()");

		r = AngelSchema::scriptEngine->RegisterObjectMethod("Performer", "int dir8()", asMETHOD(Performer, dir8), asCALL_THISCALL);
		Assert(r >= 0 && "Failed to register method dec()");

		r = AngelSchema::scriptEngine->RegisterObjectMethod("Performer", "bool hasTarget()", asMETHOD(Performer, hasTarget), asCALL_THISCALL);
		Assert(r >= 0 && "Failed to register method hasTarget()");

		return true;
	}


	int Performer
	::dir8() {
		LogFatal("TODO:");
		//TODO:
		//return BrayT::toClockwise8(actor_.pos().localFace().yaw_);
		return 0;
	}


	bool Performer
	::didMove() {
		if(actor_.didMove()) {
			LogMsg("True: " << actor_.name());
		}
		return actor_.didMove();
	}


	bool Performer
	::hasTarget() {
		return actor_.hasTarget();
	}

}
