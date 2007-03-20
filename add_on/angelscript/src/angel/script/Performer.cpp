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
#include "ScriptFunctions.hpp"
#include "../schema/AngelSchema.hpp"
#include <se_core.hpp>
#include <angelscript.h>

namespace se_core {

	Performer
	::Performer(const Composite& composite) : composite_(composite), refCount_(0) {
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

		r = AngelSchema::scriptEngine->RegisterObjectMethod("Performer", "bool hasTarget()", asMETHOD(Performer, hasTarget), asCALL_THISCALL);
		Assert(r >= 0 && "Failed to register method hasTarget()");

		r = AngelSchema::scriptEngine->RegisterObjectMethod("Performer", "float targetDistance()", asMETHOD(Performer, targetDistance), asCALL_THISCALL);
		Assert(r >= 0 && "Failed to register method targetDistance()");

		r = AngelSchema::scriptEngine->RegisterObjectMethod("Performer", "void attack()", asMETHOD(Performer, defaultAction), asCALL_THISCALL);
		Assert(r >= 0 && "Failed to register method hasTarget()");

		r = AngelSchema::scriptEngine->RegisterObjectMethod("Performer", "void log(string& s)", asMETHOD(Performer, log), asCALL_THISCALL);
		Assert(r >= 0 && "Failed to register method name()");

		r = AngelSchema::scriptEngine->RegisterObjectMethod("Performer", "void log()", asMETHOD(Performer, logSelf), asCALL_THISCALL);
		Assert(r >= 0 && "Failed to register method name()");

		r = AngelSchema::scriptEngine->RegisterObjectMethod("Performer", "void perform(string& s)", asMETHOD(Performer, perform), asCALL_THISCALL);
		Assert(r >= 0 && "Failed to register method name()");

		return true;
	}


	bool Performer
	::didMove() {
		return PhysicsComponent::Ptr(composite_)->didMove();
	}


	bool Performer
	::hasTarget() {
		return StatComponent::Ptr(composite_)->hasTarget();
	}


	float Performer
	::targetDistance() {
		StatComponent::Ptr pStat(composite_);
		if(!pStat->hasTarget()) {
			return 9999999;
		}

		Composite* target = pStat->target();
		PosComponent::Ptr pPos(composite_);
		PosComponent::Ptr tPos(*target);
		return pPos->pos().worldCoor().distance(tPos->pos().worldCoor());
	}


	void Performer
	::defaultAction() {
		StatComponent::Ptr pStat(composite_);
		if(pStat->hasDefaultAction()) {
			AngelSchema::nextAction().set( pStat->defaultAction() );
		}
		ScriptFunctions::yield();
	}


	void Performer
	::perform(const std::string& action) {
		const Action* a = SimSchema::sortedSimObjectList().action(action.c_str());
		AssertFatal(a, composite_.name() << " tried to perform unknown action " << action.c_str());
		AngelSchema::nextAction().setAction( *a );
		ScriptFunctions::yield();
	}


	void Performer
	::log(std::string& s) {
		LogMessage(composite_.name() << ": " << s.c_str());
	}


	void Performer
	::logSelf() {
		LogMessage(composite_.name());
	}
}
