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


#include "StopCutscene.hpp"
#include "sim/thing/Actor.hpp"

using namespace se_core;

namespace se_basic {

	void StopCutscene
	::perform(long when, ActionComponent& perf, Parameter& parameter) const {
		Actor& performer = *Actor::Ptr(perf);;
		LogFatal("TODO");
		// TODO:
		//performer.stopShowingCutscene();
	}


	const StopCutscene actionStopCutscene;
}

